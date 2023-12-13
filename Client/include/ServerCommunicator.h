#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <string>
#include <netinet/in.h>


class ServerCommunicator {
public:
    ServerCommunicator(const std::string& serverIP, int serverPort);
    ~ServerCommunicator();

    bool connectToServer();
    bool sendMessage(const std::string& message);
    bool sendFile(const std::string& filePath);

private:
    std::string serverIP;
    int serverPort;
    int sockfd;
    struct sockaddr_in serverAddr;

    bool isConnected;
};

#endif // SERVERCOMMUNICATOR_H
