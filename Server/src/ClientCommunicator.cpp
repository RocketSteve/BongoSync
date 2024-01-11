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

                buffer[bytesRead] = '\0'; // Null-terminate the string
                nlohmann::json messageJson = nlohmann::json::parse(buffer);
                //std::cout << "Received message: " << messageJson.dump(4) << std::endl;

                if (messageJson["action"] == "file_transfer") {
                    std::cout << "File metadata: " << messageJson << std::endl;
                    std::string filePath = messageJson["data"]["file_name"];
                    int64_t fileSize = messageJson["data"]["file_size"];

                    std::cout << "Receiving file: " << filePath << std::endl;
                    std::cout << "File size: " << fileSize << std::endl;

                    handleFileReception(clientSocket, filePath, fileSize);
                } else if (messageJson["action"] == "register") {
                    std::string email = messageJson["data"]["email"];
                    std::string password = messageJson["data"]["password"];
                    std::string hostname = messageJson["data"]["hostname"];

                    bool registrationResult = RegistrationHandler::handleRegistration(email, password, hostname);
                    MessageBuilder messageBuilder;
                    std::string responseMessage;

                    if (registrationResult) {
                        responseMessage = messageBuilder.buildUserAddedMessage();
                    } else {
                        responseMessage = messageBuilder.buildUserExistsMessage();
                    }

                    send(clientSocket, responseMessage.c_str(), responseMessage.length(), 0);
                } else if (messageJson["action"] == "login") {
                    std::string email = messageJson["data"]["email"];
                    std::string passwordHash = messageJson["data"]["password"];

                    bool loginResult = HandleLogin::checkCredentials(email, passwordHash);
                    MessageBuilder messageBuilder;
                    std::string responseMessage;

                    if (loginResult) {
                        responseMessage = messageBuilder.buildPasswordCorrectMessage();
                    } else {
                        responseMessage = messageBuilder.buildPasswordIncorrectMessage();
                    }

                    send(clientSocket, responseMessage.c_str(), responseMessage.length(), 0);
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

void ClientCommunicator::handleFileReception(int clientSocket, const std::string& originalFilePath, int64_t fileSize) {
    // Create the transfer directory if it doesn't exist
    const std::string transferDir = "./transfer/";
    mkdir(transferDir.c_str(), 0777); // 0777 is the permission mode

    // Extract filename from original file path
    std::string filename = originalFilePath.substr(originalFilePath.find_last_of("/\\") + 1);

    // Construct the new file path
    std::string newFilePath = transferDir + filename;

    std::ofstream outputFile(newFilePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << newFilePath << std::endl;
        // Optionally, send an error message to the client
        return;
    }

    int64_t totalBytesReceived = 0;
    const int bufferSize = 4096;
    char buffer[bufferSize];

    while (totalBytesReceived < fileSize) {
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




