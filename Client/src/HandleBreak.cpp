#include "../include/HandleBreak.h"

void HandleBreak::initiateBreak() {
    stopFileWatcher();
    closeConnection();
}

void stopFileWatcher() {
    FileWatcher::stop();
}

void closeConnection() {
    ServerCommunicator.getInstance().closeConnection();
}