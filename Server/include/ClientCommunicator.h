// ClientCommunicator.h

#ifndef CLIENTCOMMUNICATOR_H
#define CLIENTCOMMUNICATOR_H

#include <string>
#include <thread>
#include <vector>
#include <poll.h>

class ClientCommunicator {
public:
    ClientCommunicator(int port);
    ~ClientCommunicator();
    void start();
    void stop();

private:
    int listenPort;
    int listenSocket;
    bool running;
    std::vector<std::thread> clientThreads;

    void listenForClients();
    void handleClient(int clientSocket);
    int setupServerSocket();
    void closeServerSocket();
};

#endif // CLIENTCOMMUNICATOR_H
