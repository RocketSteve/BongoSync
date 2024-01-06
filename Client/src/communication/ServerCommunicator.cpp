#include "../include/communication/ServerCommunicator.h"


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

bool ServerCommunicator::sendFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        int len = file.gcount();
        if (send(sockfd, buffer, len, 0) == -1) {
            std::cerr << "Failed to send file data" << std::endl;
            return false;
        }
    }

    file.close();
    return true;
}

bool ServerCommunicator::receiveFile(const std::string& filePath) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    // Read the size of the file first
    int64_t fileSize = 0;
    int len = recv(sockfd, &fileSize, sizeof(fileSize), 0);
    if (len != sizeof(fileSize)) {
        std::cerr << "Failed to receive file size" << std::endl;
        return false;
    }

    int64_t remaining = fileSize;

    char buffer[1024];
    while (remaining > 0) {
        int toRead = std::min(remaining, (int64_t)sizeof(buffer));
        int bytesRead = recv(sockfd, buffer, toRead, 0);
        if (bytesRead <= 0) {
            std::cerr << "Failed to receive file data or connection closed" << std::endl;
            return false;
        }

        file.write(buffer, bytesRead);
        remaining -= bytesRead;
    }

    file.close();
    return true;
}



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

