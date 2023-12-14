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
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (context == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    const EVP_MD* algorithm = EVP_sha256();
    if (!EVP_DigestInit_ex(context, algorithm, nullptr)) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize digest context");
    }

    if (!EVP_DigestUpdate(context, password.c_str(), password.size())) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update digest");
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    if (!EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize digest");
    }

    std::stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    EVP_MD_CTX_free(context);
    return ss.str();
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

std::string Utility::getTreeHash() {
    std::string hostname = Utility::getHostname();
    std::string bongoDirectory = std::filesystem::path(getenv("HOME")) / ".bongo";
    std::string hashFilePath = bongoDirectory + "/" + hostname + ".hash";

    std::ifstream hashFile(hashFilePath);
    if (!hashFile.is_open()) {
        throw std::runtime_error("Failed to open hash file: " + hashFilePath);
    }

    std::stringstream buffer;
    buffer << hashFile.rdbuf();
    return buffer.str();
}