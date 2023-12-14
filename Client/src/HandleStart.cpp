#include "../include/HandleStart.h"



void HandleStart::initiateStart() {
    if (configExists()) {
        std::string password = Utility::promptForPassword();
        if (validateWithServer(password)) {
            getLatestVersion();
            startFileServices();
        }
    } else {
        std::cout << "No config found, please run bongo register\n";
        return;
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

    std::string email = Utility::getEmailFromConfig();
    std::string passwdHash = Utility::hashPassword(password);

    MessageBuilder messageCreator;
    std::string loginMessage = MessageCreator::create()
            .setEmail(email)
            .setPassword(passwdHash)
            .buildLoginMessage();

    auto& serverCommunicator = ServerCommunicator::getInstance();
    if (!serverCommunicator.isConnectedToServer() && !serverCommunicator.connectToServer()) {
        std::cerr << "Failed to connect to server\n";
        return false;
    }
    serverCommunicator.sendMessage(loginMessage);

    std::string responseStr;
    if (!serverCommunicator.receiveMessage(responseStr)) {
        std::cerr << "Failed to receive response from server.\n";
        return false;
    }

    // Parse the JSON response
    auto responseJson = nlohmann::json::parse(responseStr);
    bool isSuccess = responseJson["success"];
    std::string message = responseJson["message"];

    if (!isSuccess) {
        std::cout << "Server response: " << message << "\n";
        return false;
    }

    std::cout << "Server response: " << message << "\n";
    return true;
}

void HandleStart::startFileServices() {
    // Implementation of starting file services
    std::cout << "Starting file services ...\n";
}

void HandleStart::getLatestVersion() {
    // Implementation of getting latest version
    std::cout << "Getting latest version ...\n";
}