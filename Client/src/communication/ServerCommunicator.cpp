#include "../../include/communication/ServerCommunicator.h"
#include <stdexcept>
#include <iostream>

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
        std::cerr << "Failed to connect to server\n";
        return false;
    }
    isConnected = true;
    return true;
}

bool ServerCommunicator::sendMessage(const std::string& message) {
    if (!isConnected) return false;

    size_t totalSent = 0;
    size_t msgLength = message.length();
    while (totalSent < msgLength) {
        ssize_t sent = send(sockfd, message.c_str() + totalSent, msgLength - totalSent, 0);
        if (sent < 0) {
            std::cerr << "Failed to send message\n";
            return false;
        }
        totalSent += sent;
    }
    return true;
}

#include "../../include/communication/ServerCommunicator.h"
#include <fstream>
#include <iostream>

// ... other methods ...

bool ServerCommunicator::sendFile(const std::string& filePath) {
    if (!isConnected) return false;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filePath << "\n";
        return false;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Send file size first
    if (send(sockfd, &fileSize, sizeof(fileSize), 0) < 0) {
        std::cerr << "Failed to send file size\n";
        return false;
    }

    // Send file contents
    char buffer[1024];
    while (file) {
        file.read(buffer, sizeof(buffer));
        ssize_t bytesToWrite = file.gcount();

        ssize_t totalWritten = 0;
        while (totalWritten < bytesToWrite) {
            ssize_t written = send(sockfd, buffer + totalWritten, bytesToWrite - totalWritten, 0);
            if (written < 0) {
                std::cerr << "Failed to send file data\n";
                return false;
            }
            totalWritten += written;
        }
    }

    return true;
}


bool ServerCommunicator::receiveMessage(std::string& outMessage) {
    if (!isConnected) return false;

    const size_t bufferSize = 1024;
    char buffer[bufferSize];
    outMessage.clear();

    while (true) {
        ssize_t bytesReceived = recv(sockfd, buffer, bufferSize - 1, 0);
        if (bytesReceived <= 0) {
            isConnected = false;
            std::cerr << "Failed to receive message or connection closed\n";
            return false;
        }

        buffer[bytesReceived] = '\0';
        outMessage.append(buffer);

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
        std::cerr << "Failed to open file for writing: " << filePath << "\n";
        return false;
    }

    size_t fileSize;
    if (recv(sockfd, &fileSize, sizeof(fileSize), 0) <= 0) {
        isConnected = false;
        std::cerr << "Failed to receive file size\n";
        return false;
    }

    char buffer[1024];
    size_t totalBytesReceived = 0;
    while (totalBytesReceived < fileSize) {
        ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            isConnected = false;
            std::cerr << "Connection closed or error while receiving file\n";
            return false;
        }

        file.write(buffer, bytesReceived);
        totalBytesReceived += bytesReceived;
    }
    return true;
}
