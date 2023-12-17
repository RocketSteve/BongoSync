#include "../../include/commandHandlers/HandleStart.h"



void HandleStart::initiateStart() {
    if (Utility::configExists()) {
        std::string password = Utility::promptForPassword();
        if (validateWithServer(password)) {
            Utility::LogIn();
            startFileServices();
        }
    } else {
        std::cout << "No config found, please run bongo register\n";
        return;
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
    std::cout << "Starting file services ...\n";
    std::string directoryPath = Utility::getDefaultDirectory();
    startFileWatcherAndSync(directoryPath);
}

void HandleStart::startFileWatcherAndSync(const std::string& directoryPath) {
    HandleSync syncHandler(directoryPath);
    fileWatcherThread = std::thread([&]() {
        syncHandler.initiateSync(); // Initiate initial sync
        FileWatcher& watcher = FileWatcher::getInstance();
        watcher.initialize(directoryPath);
        watcher.start(); // This will run in the background
    });
}

