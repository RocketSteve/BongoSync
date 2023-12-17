#include "../include/HandleBreak.h"

void HandleBreak::initiateBreak() {
    if (!Utility::isLoggedIn()) {
        std::cout << "Not logged in, please start the application\n";
        return;
    }
    stopFileWatcher();
    closeConnection();
}

void stopFileWatcher() {
    FileWatcher::stop();
}

void closeConnection() {
    ServerCommunicator.getInstance().closeConnection();
}