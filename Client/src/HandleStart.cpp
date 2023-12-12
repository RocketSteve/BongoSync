#include "../include/HandleStart.h"
#include <iostream>

void HandleStart::initiateStart() {
    if (configExists()) {
        std::string password = promptForPassword();
        if (validateWithServer(password)) {
            getLatestVersion();
            startFileServices();
        }
    }
}

bool HandleStart::configExists() {
    // Implementation of checking if config exists
    std::cout << "Checking if config exists ...\n";
    return true;
}

std::string HandleStart::promptForPassword() {
    std::string password;
    std::cout << "Enter your password: ";
    // You might want to implement a more secure way to read the password
    std::getline(std::cin, password);
    return password;
}

bool HandleStart::validateWithServer(const std::string& password) {
    std::cout << "Validating with server ...\n" << password << "\n";

    bool response;

    if (response == false) {
        std::cout << "Password is incorrect, please try again.\n";
        return false;
    } else {
        return true;
    }
}

void HandleStart::startFileServices() {
    // Implementation of starting file services
    std::cout << "Starting file services ...\n";
}

void HandleStart::getLatestVersion() {
    // Implementation of getting latest version
    std::cout << "Getting latest version ...\n";
}