// ClientCommunicator.h

#ifndef CLIENTCOMMUNICATOR_H
#define CLIENTCOMMUNICATOR_H

#include <string>
#include <thread>
#include <vector>
#include <poll.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "Handlers/RegistrationHandler.h"
#include "Communciation/MessageBuilder.h"
#include "Handlers/HandleLogin.h"
#include "Handlers/SyncHandler.h"
#include "Communciation/Session.h"
#include "Communciation/SessionManager.h"

class ClientCommunicator {
public:
    ClientCommunicator(int port);
    ~ClientCommunicator();
    void start();
    void stop();
    sem_t semaphore;

private:
    int listenPort;
    int listenSocket;
    bool running;
    std::vector<std::thread> clientThreads;

    void listenForClients();
    void handleClient(int clientSocket);
    void handleFileReception(int clientSocket, const std::string& targetPath, const std::string& originalFilePath, int64_t fileSize);
    int setupServerSocket() const;
    void closeServerSocket() const;
};

#endif // CLIENTCOMMUNICATOR_H
