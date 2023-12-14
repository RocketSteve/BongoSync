#ifndef CLIENT_HANDLESYNC_H
#define CLIENT_HANDLESYNC_H

#include "../include/Utility.h"
#include "../include/ServerCommunicator.h"
#include "../include/MessageBuilder.h"
#include "../include/MessageCreator.h"
#include <iostream>


class HandleSync {
public:
    void initiateSync();
private:
    void checkWithServer();
    bool checkWithServer(std::string& treeHash, std::string& hostname);
    void locateConflict();
    void processServerSyncResponse(const std::string& response);
};

#endif //CLIENT_HANDLESYNC_H
