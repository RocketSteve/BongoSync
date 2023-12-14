#ifndef CLIENT_HANDLESTART_H
#define CLIENT_HANDLESTART_H
// TODO
// 1. Complete file services
// 2. Complete getLatestVersion


#include <string>
#include <iostream>
#include <filesystem>
#include "../include/Utility.h"
#include "../include/ServerCommunicator.h"
#include "../include/MessageBuilder.h"
#include "../include/MessageCreator.h"

class HandleStart {
public:
    void initiateStart();
private:
    bool configExists();
    bool validateWithServer(const std::string& password);

    void startFileServices();
    void getLatestVersion();
};

#endif //CLIENT_HANDLESTART_H
