#include "../include/ServerCommunicator.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

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

bool ServerCommunicator::sendFile(const std::string& filePath) {
    if (!isConnected) return false;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        if (send(sockfd, buffer, file.gcount(), 0) < 0) {
            return false;
        }
    }

    return true;
}
