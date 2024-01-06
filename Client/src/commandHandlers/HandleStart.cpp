#include "../../include/commandHandlers/HandleStart.h"

//TODO message interpretation

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

    // Create login message
    MessageBuilder messageCreator;
    std::string loginMessage = MessageCreator::create()
            .setEmail(email)
            .setPassword(passwdHash)
            .buildLoginMessage();

    // Get the instance of ServerCommunicator
    auto& serverCommunicator = ServerCommunicator::getInstance();

    // Check if already connected, if not, try to connect
    if (!serverCommunicator.isConnectedToServer()) {
        std::string serverIP;
        int serverPort;

        std::cout << "Enter server IP: ";
        std::cin >> serverIP;

        std::cout << "Enter server port: ";
        std::cin >> serverPort;

        if (!serverCommunicator.connectToServer(serverIP, serverPort)) {
            std::cerr << "Failed to connect to server\n";
            return false;
        }
    }

    // Send the login message
    if (!serverCommunicator.sendMessage(loginMessage)) {
        std::cerr << "Failed to send login message.\n";
        return false;
    }

    // Receive the response
    std::string responseStr;
    if (!(responseStr = serverCommunicator.receiveMessage()).empty()) {
        std::cerr << "Failed to receive response from server.\n";
        return false;
    }

    // Handle the response
    try {
        auto responseJson = nlohmann::json::parse(responseStr);
        bool isSuccess = responseJson.value("success", false);
        std::string message = responseJson.value("message", "");

        std::cout << "Server response: " << message << "\n";
        return isSuccess;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
        return false;
    }
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

