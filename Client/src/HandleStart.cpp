#include "../include/HandleStart.h"


void HandleStart::initiateStart() {
    if (configExists()) {
        std::string password = Utility::promptForPassword();
        if (validateWithServer(password)) {
            getLatestVersion();
            startFileServices();
        }
    }
}

bool HandleStart::configExists() {
    std::cout << "Checking if config exists ...\n";
    if (std::filesystem::exists(std::string(getenv("HOME")) + "/.bongo/config.json")) {
        return true;
    } else {
        return false;
    }
}

bool HandleStart::validateWithServer(const std::string& password) {
    std::cout << "Validating with server ...\n" << password << "\n";

    bool response;
    MessageCreator msgCreator;
    std::string email = Utility::getEmailFromConfig();
// Implementation of validating with server
    std::string passwdHash = Utility::hashPassword(password);
    std::string msg = msgCreator.createLoginMessage(email, passwdHash);

    auto& serverCommunicator = ServerCommunicator::getInstance();
    if (!serverCommunicator.isConnectedToServer() && !serverCommunicator.connectToServer()) {
        std::cerr << "Failed to connect to server\n";
        return false;
    }
    serverCommunicator.sendMessage(msg);

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