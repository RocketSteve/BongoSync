#include "../include/ClientCommunicator.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <nlohmann/json.hpp>

ClientCommunicator::ClientCommunicator(int port) : listenPort(port), listenSocket(-1), running(false) {}

ClientCommunicator::~ClientCommunicator() {
    stop();
}

void ClientCommunicator::start() {
    listenSocket = setupServerSocket();
    if (listenSocket == -1) {
        std::cerr << "Failed to set up server socket." << std::endl;
        return;
    }

    running = true;
    std::thread listeningThread(&ClientCommunicator::listenForClients, this);
    listeningThread.detach();
}

void ClientCommunicator::stop() {
    running = false;
    closeServerSocket();
    for (auto& thread : clientThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

int ClientCommunicator::setupServerSocket() const {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    struct sockaddr_in serverAddr{};
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(listenPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    std::cout << "Binding to port " << listenPort << std::endl;
    std::cout << "Binding to address " << (struct sockaddr *)&serverAddr << std::endl;

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to bind socket." << std::endl;
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 10) == -1) {
        std::cerr << "Failed to listen on socket." << std::endl;
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void ClientCommunicator::closeServerSocket() const {
    if (listenSocket != -1) {
        close(listenSocket);
    }
}

void ClientCommunicator::listenForClients() {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    while (running) {
        int clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            if (running) {
                std::cerr << "Failed to accept client." << std::endl;
            }
            continue;
        }

        clientThreads.emplace_back(&ClientCommunicator::handleClient, this, clientSocket);
    }
}


void ClientCommunicator::handleClient(int clientSocket) {
    const int bufferSize = 4096;
    char buffer[bufferSize];
    struct pollfd fd;
    fd.fd = clientSocket;
    fd.events = POLLIN;

    std::string userEmail = "";
    std::string hostname = "";

    while (true) {
        int ret = poll(&fd, 1, 5000);
        if (ret > 0) {
            if (fd.revents & POLLIN) {
                ssize_t bytesRead = recv(clientSocket, buffer, bufferSize - 1, 0);
                if (bytesRead <= 0) {
                    if (bytesRead == 0) {
                        std::cout << "Client disconnected." << std::endl;
                    } else {
                        std::cerr << "Error receiving data." << std::endl;
                    }
                    break;
                }

                std::cout << "Received " << bytesRead << " bytes" << std::endl;
                buffer[bytesRead] = '\0'; // Null-terminate the string
                nlohmann::json messageJson = nlohmann::json::parse(buffer);
                std::cout << "Received message: " << messageJson.dump(4) << std::endl;

                if (messageJson["action"] == "file_transfer") {
                    std::cout << "File metadata: " << messageJson << std::endl;
                    std::string filePath = messageJson["data"]["file_name"];
                    int64_t fileSize = messageJson["data"]["file_size"];


                    std::cout << "Receiving file: " << filePath << std::endl;
                    std::cout << "File size: " << fileSize << std::endl;
                    handleFileReception(clientSocket,"files", filePath, fileSize);

                } else if (messageJson["action"] == "register") {
                    std::string email = messageJson["data"]["email"];
                    std::string password = messageJson["data"]["password"];
                    std::string hostname = messageJson["data"]["hostname"];
                    std::string treeHash = messageJson["data"]["tree_hash"];

                    bool registrationResult = RegistrationHandler::handleRegistration(email, password, hostname, treeHash);
                    MessageBuilder messageBuilder;
                    std::string responseMessage;

                    if (registrationResult) {
                        // Code to create a directory named after the hostname
                        std::filesystem::path dirPath = std::filesystem::current_path() / hostname;
                        if (!std::filesystem::exists(dirPath)) {
                            // Create the directory if it doesn't exist
                            try {
                                std::filesystem::create_directory(dirPath);
                                std::cout << "Created directory: " << dirPath << std::endl;
                            } catch (const std::filesystem::filesystem_error& e) {
                                std::cerr << "Error creating directory: " << e.what() << std::endl;
                            }
                        }
                        responseMessage = messageBuilder.buildUserAddedMessage();
                    } else {
                        responseMessage = messageBuilder.buildUserExistsMessage();
                    }

                    send(clientSocket, responseMessage.c_str(), responseMessage.length(), 0);
                } else if (messageJson["action"] == "login") {
                    std::string email = messageJson["data"]["email"];
                    std::string passwordHash = messageJson["data"]["password"];

                    userEmail = email;

                    std::string query = "SELECT hostname FROM app_users WHERE email = $1;";
                    std::vector<std::string> values = {userEmail};

                    DatabaseManager& dbManager = DatabaseManager::getInstance();
                    hostname = dbManager.readCustomRecord(query, values);

                    std::cout << "Hostname: " << hostname << std::endl;

                    bool loginResult = HandleLogin::checkCredentials(email, passwordHash);

                    MessageBuilder messageBuilder;
                    std::string responseMessage;

                    if (loginResult) {
                        std::cout << "Login successful" << std::endl;
                        responseMessage = messageBuilder.buildPasswordCorrectMessage();
                    } else {
                        std::cout << "Login failed" << std::endl;
                        responseMessage = messageBuilder.buildPasswordIncorrectMessage();
                    }

                    send(clientSocket, responseMessage.c_str(), responseMessage.length(), 0);
                } else if (messageJson["action"] == "check_latest") {
                    std::cout << "Checking if latest" << std::endl;
                    std::string treeHash = messageJson["data"]["tree_hash"];
                    std::string modifiedAt = messageJson["data"]["modified_at"];

                    bool isLatest = SyncHandler::isLatestVersion(treeHash, userEmail, modifiedAt);

                    MessageBuilder messageBuilder;
                    std::string responseMessage;

                    if (isLatest) {
                        responseMessage = messageBuilder.buildLatestVersionMessage();
                        std::cout << "Latest version" << std::endl;
                    } else {
                        std::cout << "Not latest version" << std::endl;
                        std::string ahead = SyncHandler::compareTimestamps(userEmail, modifiedAt);
                        std::cout << ahead << " is ahead" << std::endl;

                        messageBuilder.setAhead(ahead);

                        std::cout << "Sending " << ahead << " is ahead message" << std::endl;

                        responseMessage = messageBuilder.buildNotLatestVersionMessage();

                        std::cout << "Message built" << std::endl;
                    }

                    send(clientSocket, responseMessage.c_str(), responseMessage.length(), 0);
                    std::cout << "Message sent" << std::endl;
                } else if (messageJson["action"] == "tree") {
                    std::cout << "Receiving Merkle tree" << std::endl;
                    bool ahead = messageJson["data"]["ahead"];
                    std::cout << "ahead: " << ahead << std::endl;
                    std::string filePath = messageJson["data"]["file_name"];
                    std::cout << "filePath: " << filePath << std::endl;
                    int64_t fileSize = messageJson["data"]["file_size"];
                    std::cout << "fileSize: " << fileSize << std::endl;

                    std::cout << "ahead: " << ahead << std::endl;
                    //std::string ahead = "client";

                    std::cout << "Receiving file: " << filePath << std::endl;
                    std::cout << "File size: " << fileSize << std::endl;

                    handleFileReception(clientSocket, "tree", filePath, fileSize);

                    std::vector<std::string> addedFiles = SyncHandler::treeReception(ahead, userEmail);
                    std::cout << "Merkle tree received" << std::endl;

                    if (addedFiles.empty()) {
                        std::cout << "No files to send" << std::endl;

                        // Create a JSON message indicating that no sync is needed
                        nlohmann::json noSyncNeededJson;
                        noSyncNeededJson["action"] = "no_sync_needed";
                        std::string noSyncNeededMessage = noSyncNeededJson.dump();

                        // Send the message to the client
                        send(clientSocket, noSyncNeededMessage.c_str(), noSyncNeededMessage.length(), 0);

                        continue;
                    } else {

                        for (size_t i = 0; i < addedFiles.size(); ++i) {
                            nlohmann::json requestFileJson;
                            requestFileJson["action"] = "request_file";
                            requestFileJson["data"]["filePath"] = addedFiles[i];
                            requestFileJson["data"]["remaining_files"] = addedFiles.size() - i;
                            std::cout << "Sending request for file: " << requestFileJson << std::endl;
                            std::string requestFileMessage = requestFileJson.dump();
                            send(clientSocket, requestFileMessage.c_str(), requestFileMessage.length(), 0);

                            // Wait for a "file_transfer" message from the client
                            nlohmann::json fileMetadataJson;
                            ssize_t bytesRead = recv(clientSocket, buffer, bufferSize - 1, 0);
                            if (bytesRead > 0) {
                                buffer[bytesRead] = '\0'; // Null-terminate the string
                                fileMetadataJson = nlohmann::json::parse(buffer);
                                if (fileMetadataJson["action"] == "file_transfer") {
                                    std::cout << "Received file metadata from client: " << fileMetadataJson << std::endl;

                                    // Extract file size and file path from the file metadata
                                    int64_t fileSize = fileMetadataJson["data"]["file_size"];
                                    std::string originalFilePath = fileMetadataJson["data"]["file_name"];

                                    // Receive the file from the client
                                    handleFileReception(clientSocket, hostname, originalFilePath, fileSize);
                                }
                            }
                        }
                    }
                    MerkleTree localTree;
                    localTree.buildTree(hostname);

                    WorkspaceManager workspaceManager;
                    std::string newWorkspaceHash = localTree.getTreeHash();
                    workspaceManager.updateWorkspaceByEmail(userEmail, newWorkspaceHash);


                } else {
                    std::string serverMessage = R"({"message": "Hello from Server!"})";
                    std::cout << "Sending message: " << serverMessage << std::endl;
                    send(clientSocket, serverMessage.c_str(), serverMessage.length(), 0);
                }
            }
        } else if (ret == 0) {
            std::cout << "No data received. Still waiting..." << std::endl;
        } else {
            std::cerr << "Poll error." << std::endl;
            break;
        }
    }

    close(clientSocket); // Close the socket after communication ends
}

void ClientCommunicator::handleFileReception(int clientSocket, const std::string& targetPath, const std::string& originalFilePath, int64_t fileSize) {
    // Ensure the target directory exists
    std::filesystem::path targetDir = std::filesystem::path(targetPath);
    std::filesystem::create_directories(targetDir);

    // Extract filename from original file path
    std::string filename = originalFilePath.substr(originalFilePath.find_last_of("/\\") + 1);

    // Construct the new file path
    std::filesystem::path newFilePath = targetDir / filename;

    std::cout << "Receiving file to: " << newFilePath << std::endl;

    std::ofstream outputFile(newFilePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << newFilePath << std::endl;
        return;
    }


    int64_t totalBytesReceived = 0;
    const int bufferSize = 4096;
    char buffer[bufferSize];

    while (totalBytesReceived < fileSize) {
        std::cout << "Received bytes: " << totalBytesReceived << std::endl;
        ssize_t bytesRead = recv(clientSocket, buffer, std::min(fileSize - totalBytesReceived, (int64_t)bufferSize), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving file data or connection closed" << std::endl;
            // Optionally, send an error message to the client
            outputFile.close();
            return;
        }

        outputFile.write(buffer, bytesRead);
        totalBytesReceived += bytesRead;
    }

    outputFile.close();

    // After successful file reception, send confirmation back to client
    nlohmann::json confirmationJson;
    confirmationJson["data"]["status"] = "success";
    confirmationJson["data"]["message"] = "File received successfully";
    std::string confirmationMessage = confirmationJson.dump();
    send(clientSocket, confirmationMessage.c_str(), confirmationMessage.length(), 0);
}




