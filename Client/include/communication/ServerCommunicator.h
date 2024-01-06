#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <string>
#include <vector>
#include <poll.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdint>
#include <algorithm>


class ServerCommunicator {
public:
    static ServerCommunicator& getInstance();
    bool connectToServer(const std::string& serverIP, int serverPort);
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    bool sendFile(const std::string& filePath);
    bool receiveFile(const std::string& filePath);
    void closeConnection();

    bool isConnectedToServer() const;

private:
    ServerCommunicator() {}
    ~ServerCommunicator() { closeConnection(); }

    bool isConnected;

    ServerCommunicator(const ServerCommunicator&) = delete;
    ServerCommunicator& operator=(const ServerCommunicator&) = delete;

    int sockfd;
    struct pollfd poll_fd;

    bool setupSocket(const std::string& serverIP, int serverPort);
    bool isSocketReadyForIO();
};

#endif // SERVERCOMMUNICATOR_H
