#include "../../include/commandHandlers/HandleBreak.h"

void HandleBreak::initiateBreak() {
    if (!Utility::isLoggedIn()) {
        std::cout << "Nothing to stop, please start the application\n";
        return;
    }
    stopFileWatcher();
    closeConnection();
    Utility::LogOut(); // Assuming Utility class has a method to handle logout
}

void HandleBreak::stopFileWatcher() {
    FileWatcher& watcher = FileWatcher::getInstance();
    watcher.stop();
}

void HandleBreak::closeConnection() {
    auto& serverCommunicator = ServerCommunicator::getInstance();
    serverCommunicator();
}
