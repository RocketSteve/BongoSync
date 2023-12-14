#include "../include/ServerCommunicator.h"

ServerCommunicator::ServerCommunicator(const std::string& ip, int port)
        : serverIP(ip), serverPort(port), isConnected(false) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);
}

ServerCommunicator::~ServerCommunicator() {
    if (isConnected) {
        close(sockfd);
    }
}

bool ServerCommunicator::connectToServer() {
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        isConnected = false;
        return false;
    }
    isConnected = true;
    return true;
}

bool ServerCommunicator::sendMessage(const std::string& message) {
    if (!isConnected) return false;
    if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
        return false;
    }
    return true;
}

bool ServerCommunicator::receiveMessage(std::string& outMessage) {
    if (!isConnected) return false;

    const size_t bufferSize = 1024;
    char buffer[bufferSize];
    outMessage.clear();

    // Example: Read until newline character
    while (true) {
        ssize_t bytesReceived = recv(sockfd, buffer, bufferSize - 1, 0);
        if (bytesReceived <= 0) {
            // Connection closed or error
            isConnected = false;
            return false;
        }

        buffer[bytesReceived] = '\0';
        outMessage.append(buffer);

        // Check for end-of-message (e.g., newline character)
        if (outMessage.find('\n') != std::string::npos) {
            break;
        }
    }
    return true;
}

bool ServerCommunicator::receiveFile(const std::string& filePath) {
    if (!isConnected) return false;

    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Example: Expect to receive file size first
    size_t fileSize;
    if (recv(sockfd, &fileSize, sizeof(fileSize), 0) <= 0) {
        isConnected = false;
        return false;
    }

    char buffer[1024];
    size_t totalBytesReceived = 0;
    while (totalBytesReceived < fileSize) {
        ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // Connection closed or error
            isConnected = false;
            return false;
        }

        file.write(buffer, bytesReceived);
        totalBytesReceived += bytesReceived;
    }
    return true;
}


