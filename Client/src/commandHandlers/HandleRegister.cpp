#include "../../include/commandHandlers/HandleRegister.h"

//TODO implement user exists message from server


void HandleRegister::initiateRegistration() {

    if (Utility::configExists()) {
        std::cout << "This computer is already registered, please start the application with:\n bongo start\n";
    }

    std::string email = Utility::promptForEmail();
    std::string password;
    std::string hostname = Utility::getHostname();

    do {
        password = Utility::promptForPassword();
    } while (!Utility::confirmPassword(password));

    if (!checkWithServer(email, password, hostname)) {
        std::cout << "User already exists, please try again.\n";
        initiateRegistration();
    } else {
        createConfigFile(email, hostname);
        std::cout << "User registered successfully.\n";
    }
}

bool HandleRegister::checkWithServer(const std::string& email, const std::string& password, const std::string& hostname) {
    // Implementation of checking with server
    std::cout << "Checking with server ...\n" << email << "\n" << password << "\n" << hostname << "\n";

    bool response;
    auto& communicator = ServerCommunicator::getInstance();
    if (!communicator.isConnectedToServer() && !communicator.connectToServer()) {
        std::cerr << "Failed to connect to server\n";
        return false;
    }

    MessageBuilder messageCreator;
    std::string regMessage = MessageCreator::create()
            .setEmail(email)
            .setHostname(hostname)
            .setPassword(password)
            .buildRegistrationMessage();
    response = communicator.sendMessage(regMessage);

    if (response) {
        return true;
    } else {
        return false;
    }
}

void HandleRegister::createConfigFile(const std::string& email, const std::string& hostname) {
    const std::string filesDir = std::filesystem::current_path();
    const std::string configDir = std::filesystem::path(getenv("HOME")) / ".bongo";
    const std::string configFile = configDir + "/config.json";

    // Create the hidden directory if it does not exist
    std::filesystem::create_directories(configDir);

    // Write email and hostname to the config file
    std::ofstream out(configFile);
    if (out) {
        out << "Email: " << email << "\n";
        out << "Hostname: " << hostname << "\n";
        out << "Path: " << filesDir << "\n";
        std::cout << "Configuration file created at: " << configFile << std::endl;
    } else {
        std::cerr << "Failed to create configuration file at: " << configFile << std::endl;
    }
}