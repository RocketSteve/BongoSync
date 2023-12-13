#include "../include/HandleRegister.h"
#include "../include/ServerCommunicator.h"
#include "../include/MessageCreator.h"
#include <iostream>
#define HOST_NAME_MAX 255

void HandleRegister::initiateRegistration() {
    std::string email = promptForEmail();
    std::string password;
    std::string hostname = getHostname();

    do {
        password = promptForPassword();
    } while (!confirmPassword(password));

    if (checkWithServer(email, password, hostname)) {
        std::cout << "User already exists, please try again.\n";
        initiateRegistration();
    } else {
        createConfigFile(email, hostname);
    }
}

std::string HandleRegister::promptForEmail() {
    std::string email;
    std::cout << "Enter your email: ";
    std::getline(std::cin, email);
    return email;
}

std::string HandleRegister::promptForPassword() {
    std::string password;
    std::cout << "Enter your password: ";

    // Turning off terminal echoing
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::getline(std::cin, password);

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    std::cout << std::endl;
    return password;
}

bool HandleRegister::confirmPassword(const std::string& password) {
    std::string confirmPassword;
    std::cout << "Confirm your password: ";

    // Turning off terminal echoing
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::getline(std::cin, confirmPassword);

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;

    if (password != confirmPassword) {
        std::cout << "Passwords do not match, please try again.\n";
        return false;
    }
    return true;
}

std::string HandleRegister::getHostname() {
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        std::cerr << "Error getting hostname: " << std::strerror(errno) << '\n';
        return "unknown";
    }
    return std::string(hostname);
}

bool HandleRegister::checkWithServer(const std::string& email, const std::string& password, const std::string& hostname) {
    // Implementation of checking with server
    std::cout << "Checking with server ...\n" << email << "\n" << password << "\n" << hostname << "\n";

    bool response;
    ServerCommunicator serverCommunicator("10.0.0.420", 420);
    if (serverCommunicator.connectToServer()) {
        MessageCreator messageCreator;
        std::string message = messageCreator.createRegistratioMessage(email, password, hostname);
        response = serverCommunicator.sendMessage(message);
    } else {
        std::cerr << "Failed to connect to server\n";
        return false;
    }

    if (response) {
        return true;
    } else {
        return false;
    }
}

void HandleRegister::createConfigFile(const std::string& email, const std::string& hostname) {
    const std::string filesDir = std::filesystem::current_path();
    const std::string configDir = std::filesystem::path(getenv("HOME")) / ".bongo";
    const std::string configFile = configDir / "config.txt";

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
