#include "../../include/commandHandlers/HandleShare.h"
#include <iostream>

void HandleShare::initiateShare(const std::string& email) {
    if (Utility::isLoggedIn()) {

        if (checkWithServer(email)) {
            std::cout << "Share initiated ...\n" << email << "\n";
        } else {
            std::cout << "Share not initiated ...\n" << email << "\n";
        }
        std::cout << "Initiating share ...\n" << email << "\n";
    } else {
    std::cout << "Not logged in, please start the application\n";
    return;
    }
}

bool HandleShare::checkWithServer(const std::string& email) {
    // Implementation of checking with server
    bool response;

    std::cout << "Checking with server ...\n" << email << "\n";
    return response;
}