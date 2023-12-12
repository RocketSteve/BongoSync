#include "../include/HandleShare.h"
#include <iostream>

void HandleShare::initiateShare(const std::string& email) {

    if (checkWithServer(email)) {
        std::cout << "Share initiated ...\n" << email << "\n";
    } else {
        std::cout << "Share not initiated ...\n" << email << "\n";
    }
    std::cout << "Initiating share ...\n" << email << "\n";
}

bool HandleShare::checkWithServer(const std::string& email) {
    // Implementation of checking with server
    bool response;

    std::cout << "Checking with server ...\n" << email << "\n";
    return response;
}