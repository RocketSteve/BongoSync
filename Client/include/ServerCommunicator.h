#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <string>
#include <netinet/in.h>

class ServerCommunicator {
public:
    static ServerCommunicator& getInstance(const std::string& serverIP = "", int serverPort = 0) {
        static ServerCommunicator instance(serverIP, serverPort);
        return instance;
    }

    // Delete the copy constructor and copy assignment operator
    ServerCommunicator(const ServerCommunicator&) = delete;
    ServerCommunicator& operator=(const ServerCommunicator&) = delete;

    bool connectToServer();
    bool isConnectedToServer() const { return isConnected; }
    bool sendMessage(const std::string& message);
    bool sendFile(const std::string& filePath);


private:
    ServerCommunicator(const std::string& serverIP, int serverPort); // Private constructor
    ~ServerCommunicator();

    std::string serverIP;
    int serverPort;
    int sockfd;
    struct sockaddr_in serverAddr;

    bool isConnected;
};

#endif // SERVERCOMMUNICATOR_H
