#include "../../include/commandHandlers/HandleBreak.h"

void HandleBreak::initiateBreak() {
    stopFileWatcher();
    closeConnection();
}

void HandleBreak::stopFileWatcher() {
    FileWatcher& watcher = FileWatcher::getInstance();
    watcher.stop();
}

void HandleBreak::closeConnection() {
    auto& serverCommunicator = ServerCommunicator::getInstance();
    serverCommunicator.closeConnection();
}
