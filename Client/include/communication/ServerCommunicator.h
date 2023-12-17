#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

// TODO revise the recieval and sending of messages and files make sure to ise TCP

#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>


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
    bool receiveMessage(std::string& outMessage);
    bool receiveFile(const std::string& filePath);
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
