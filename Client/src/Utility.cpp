#include "../include/Utility.h"


std::string Utility::promptForEmail() {
    std::string email;
    std::cout << "Enter your email: ";
    std::cin >> email;
    return email;
}

std::string Utility::getHostname() {
    std::string hostname;
    std::cout << "Enter your hostname: ";
    std::cin >> hostname;
    return hostname;
}

std::string Utility::getEmailFromConfig() {
    std::ifstream configFile;
    configFile.open("config.json");
    nlohmann::json j;
    configFile >> j;
    return j["email"];
}

std::string Utility::hashPassword(const std::string& password) {
    return BCrypt::generateHash(password, BCrypt::getRandomSalt());
}

std::string Utility::promptForPassword() {
    std::string password;
    std::cout << "Enter your password: ";

    // Turning off terminal echoing
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::cin >> password;

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    std::cout << std::endl;
    return password;
}

bool Utility::confirmPassword(const std::string& password) {
    std::string confirmPassword;
    std::cout << "Confirm your password: ";

    // Turning off terminal echoing
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::cin >> confirmPassword;

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;

    return password == confirmPassword;
}