#ifndef CLIENT_HANDLESTART_H
#define CLIENT_HANDLESTART_H
// TODO
// 1. Complete file services
// 2. Complete getLatestVersion


#include <string>
#include <iostream>
#include <filesystem>
#include "../Utility.h"
#include "../communication/ServerCommunicator.h"
#include "../communication/MessageBuilder.h"
#include "../communication/MessageCreator.h"
#include "../fileServices/FileWatcher.h"
#include "../fileServices/ConflictResolver.h"
#include "HandleSync.h"
#include <thread>

class HandleStart {
public:
    void initiateStart();
    void startFileServices();
    void startFileWatcherAndSync(const std::string& directoryPath);
    std::thread fileWatcherThread;
private:
    bool validateWithServer(const std::string& password);
};

#endif //CLIENT_HANDLESTART_H
