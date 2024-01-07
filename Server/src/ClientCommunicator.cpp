#include "../include/ClientCommunicator.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
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

int ClientCommunicator::setupServerSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    struct sockaddr_in serverAddr;
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

void ClientCommunicator::closeServerSocket() {
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
    char buffer[4096]; // Adjust buffer size as necessary
    ssize_t bytesRead;

    // Read data from the client
    bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string

        // Parse the JSON message
        std::string messageStr(buffer);
        nlohmann::json messageJson = nlohmann::json::parse(messageStr);

        // Print the message
        std::cout << "Received message: " << messageJson.dump(4) << std::endl;
    }

    close(clientSocket);
}


