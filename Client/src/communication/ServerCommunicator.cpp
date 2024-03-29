#include "../../include/communication/ServerCommunicator.h"
#include <thread>


ServerCommunicator& ServerCommunicator::getInstance() {
    static ServerCommunicator instance;
    return instance;
}

bool ServerCommunicator::connectToServer(const std::string& serverIP, int serverPort) {
    if (!setupSocket(serverIP, serverPort)) {
        return false;
    }

    poll_fd.fd = sockfd;
    poll_fd.events = POLLIN | POLLOUT;

    isConnected = true;
    return true;
}

bool ServerCommunicator::sendMessage(const std::string& message) {
    if (!isSocketReadyForIO()) {
        return false;
    }
    return send(sockfd, message.c_str(), message.size(), 0) != -1;
}

std::string ServerCommunicator::receiveMessage() {
    if (!isSocketReadyForIO()) {
        return "";
    }

    char buffer[1024];
    int len = recv(sockfd, buffer, sizeof(buffer), 0);
    if (len > 0) {
        return std::string(buffer, len);
    }
    return "";
}

bool ServerCommunicator::receiveFile() {
    std::cout << "Receiving file from server...\n";
    //send file_transfer message to server
    nlohmann::json fileTransferMsg;
    fileTransferMsg["action"] = "file_transfer";
    sendMessage(fileTransferMsg.dump());

    for(int i = 0; i < 5; i++) {
        std::string metadataMsg = receiveMessage();
        if (metadataMsg.empty()) {
            std::cerr << "Failed to receive file metadata\n";
            return false;
        }

        nlohmann::json fileMetadata = nlohmann::json::parse(metadataMsg);
        std::string originalFilePath = fileMetadata["data"]["file_name"];
        int64_t fileSize = fileMetadata["data"]["file_size"];
        std::cout << "Receiving file: " << originalFilePath << " (" << fileSize << " bytes)\n";

        std::filesystem::path safeFilePath = std::filesystem::path(originalFilePath).filename();

        std::filesystem::path baseDirectory = "files";
        std::filesystem::path fullFilePath = baseDirectory / safeFilePath;

        std::filesystem::create_directories(baseDirectory);

        std::ofstream file(fullFilePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << fullFilePath << "\n";
            return false;
        }

        int64_t remaining = fileSize;
        char buffer[4096]; // Using a larger buffer for efficiency
        while (remaining > 0) {
            int toRead = std::min(remaining, (int64_t)sizeof(buffer));
            int bytesRead = recv(sockfd, buffer, toRead, 0);
            if (bytesRead <= 0) {
                std::cerr << "Failed to receive file data or connection closed\n";
                file.close();
                // Optionally, remove the partially written file
                std::filesystem::remove(fullFilePath);
                return false;
            }

            file.write(buffer, bytesRead);
            remaining -= bytesRead;
        }

        file.close();
    }
    std::cout << "Files received successfully\n";
    return true;
}

bool ServerCommunicator::sendFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    int64_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Send file metadata as JSON
    nlohmann::json fileMetadata;
    fileMetadata["action"] = "file_transfer";
    fileMetadata["data"]["file_name"] = filePath;
    fileMetadata["data"]["file_size"] = fileSize;
    sendMessage(fileMetadata.dump());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Stream file data
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        int len = file.gcount();
        if (send(sockfd, buffer, len, 0) == -1) {
            std::cerr << "Failed to send file data" << std::endl;
            file.close();
            return false;
        }
    }

    file.close();

    // Wait for confirmation message
    std::string confirmationResponse = receiveMessage();
    if (confirmationResponse.empty()) {
        std::cerr << "No confirmation received" << std::endl;
        return false;
    }

    nlohmann::json confirmationJson = nlohmann::json::parse(confirmationResponse);
    if (confirmationJson["data"]["status"] != "success") {
        std::string errorMessage = "File transfer failed";
        if (confirmationJson["data"].contains("message") && confirmationJson["data"]["message"].is_string()) {
            errorMessage += ": " + confirmationJson["data"]["message"].get<std::string>();
        }
        std::cerr << errorMessage << std::endl;
        return false;
    } else {
        std::cout << "File transfer successful" << std::endl;
    }
    
    return true;
}

// bool ServerCommunicator::receiveFile(std::string& filePath) {
//     return true;
// }
//bool ServerCommunicator::receiveFile() {
//    // Receive the JSON metadata message
//    std::string metadataMsg = receiveMessage();
//    if (metadataMsg.empty()) {
//        std::cerr << "Failed to receive file metadata" << std::endl;
//        return false;
//    }
//
//    // Parse JSON metadata
//    nlohmann::json fileMetadata = nlohmann::json::parse(metadataMsg);
//    std::string filePath = fileMetadata["data"]["file_name"];
//    int64_t fileSize = fileMetadata["data"]["file_size"];
//
//    std::ofstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
//        return false;
//    }
//
//    // Receive file data
//    int64_t remaining = fileSize;
//    char buffer[1024];
//    while (remaining > 0) {
//        int toRead = std::min(remaining, (int64_t)sizeof(buffer));
//        int bytesRead = recv(sockfd, buffer, toRead, 0);
//        if (bytesRead <= 0) {
//            std::cerr << "Failed to receive file data or connection closed" << std::endl;
//
//            nlohmann::json errorMsg;
//            errorMsg["action"] = "file_transfer_confirmation";
//            errorMsg["data"]["file_name"] = filePath;
//            errorMsg["data"]["status"] = "error";
//
//            sendMessage(errorMsg.dump());
//
//            file.close();
//            return false;
//        }
//
//        file.write(buffer, bytesRead);
//        remaining -= bytesRead;
//    }
//
//    nlohmann::json confirmationMsg;
//    confirmationMsg["action"] = "file_transfer_confirmation";
//    confirmationMsg["data"]["file_name"] = filePath;
//    confirmationMsg["data"]["status"] = "success";
//    sendMessage(confirmationMsg.dump());
//
//    file.close();
//    return true;
//}




void ServerCommunicator::closeConnection() {
    if (sockfd) {
        close(sockfd);
        isConnected = false;
        sockfd = 0;
    }
}

bool ServerCommunicator::setupSocket(const std::string& serverIP, int serverPort) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        return false;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(serverPort);
    server_addr.sin_addr.s_addr = inet_addr(serverIP.c_str());

    return connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != -1;
}

bool ServerCommunicator::isSocketReadyForIO() {
    int ret = poll(&poll_fd, 1, 5000); // 5000 ms timeout
    if (ret <= 0) {
        return false; // Error or timeout
    }
    if (poll_fd.revents & (POLLIN | POLLOUT)) {
        return true; // Socket ready for read/write
    }
    return false;
}

bool ServerCommunicator::isConnectedToServer() const {
    return isConnected;
}

bool ServerCommunicator::isMessageReady() {
    struct pollfd fdArray[1];
    fdArray[0].fd = sockfd;
    fdArray[0].events = POLLIN;

    int ret = poll(fdArray, 1, 0);
    return (ret > 0) && (fdArray[0].revents & POLLIN);
}

bool ServerCommunicator::checkForIncomingMessages() {
    struct pollfd fdArray[1];
    fdArray[0].fd = sockfd;
    fdArray[0].events = POLLIN;

    int ret = poll(fdArray, 1, 5000); // 5000 ms timeout
    return (ret > 0) && (fdArray[0].revents & POLLIN);
}

void ServerCommunicator::processIncomingMessages() {
    if (checkForIncomingMessages()) {
        std::string message = receiveMessage();
        if (!message.empty()) {
            // Process the received message
            std::cout << "Received message: " << message << std::endl;
            // Additional processing logic here...
        }
    }
}

bool ServerCommunicator::sendMerkleTreeFile(std::string &ahead) {
    bool clientAhead = (ahead == "client");
    // Base directory
    std::string baseDirectory = Utility::getBongoDir();

    // Relative path of the Merkle tree file
    std::string relativeFilePath = "/tree.json";

    // Full path to the Merkle tree file
    std::string fullPath = baseDirectory + relativeFilePath;

    // Open the file
    std::ifstream file(fullPath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fullPath << std::endl;
        return false;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    int64_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Create an instance of MessageBuilder
    MessageBuilder messageBuilder;

    // Set the necessary fields
    messageBuilder.setRelativeFilePath(relativeFilePath)
            .setAhead(clientAhead)
            .setFileSize(fileSize);

    // Build file metadata message
    std::string fileMetadata = messageBuilder.buildFileMetadataMessage();

    std::cout << "Sending file metadata: " << fileMetadata << std::endl;


    // Get the instance of ServerCommunicator
    auto& serverCommunicator = ServerCommunicator::getInstance();

    // Send file metadata
    if (!serverCommunicator.sendMessage(fileMetadata)) {
        std::cerr << "Failed to send file metadata." << std::endl;
        file.close();
        return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Stream file data
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        int len = file.gcount();
        if (send(sockfd, buffer, len, 0) == -1) {
            std::cerr << "Failed to send file data" << std::endl;
            file.close();
            return false;
        }
    }

    file.close();

    // Wait for confirmation message
    std::string confirmationResponse = serverCommunicator.receiveMessage();
    if (confirmationResponse.empty()) {
        std::cerr << "No confirmation received" << std::endl;
        return false;
    }

    nlohmann::json confirmationJson = nlohmann::json::parse(confirmationResponse);
    if (confirmationJson["data"]["status"] != "success") {
        std::cerr << "File transfer failed: " << confirmationJson["data"]["message"].get<std::string>() << std::endl;
        return false;
    } else {
        std::cout << "File transfer successful" << std::endl;
    }

    return true;
}

