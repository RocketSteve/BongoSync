#include "../include/HandleUnshare.h"

// TODO implement this class

void HandleUnshare::initiateUnshare(const std::string& email) {
    // Implementation of unsharing

    if (checkWithServer(email)) {
        std::cout << "Unshared with " << email << std::endl;
    } else {
        std::cout << "Unshare failed" << std::endl;
    }
}

bool HandleUnshare::checkWithServer(const std::string& email) {
    // Implementation of checking with server
    bool response;

    std::cout << "Checking with server ...\n" << email << "\n";

    return response;
}