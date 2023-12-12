#include "../include/HandleSync.h"
#include <iostream>

void HandleSync::initiateSync() {
    std::string email = promptForEmail();
    std::string password = promptForPassword();
    std::string hostname = getHostname();

    if (checkWithServer(email, password, hostname)) {
        std::cout << "User does not exist, please try again.\n";
        initiateSync();
    } else {
        std::cout << "Syncing ...\n";
        std::cout << "Sync complete.\n";
    }
}