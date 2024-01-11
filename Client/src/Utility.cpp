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
    configFile.open(std::filesystem::path(getenv("HOME")) / ".bongo/config.json");
    std::cout << "Reading config file ...\n";
    nlohmann::json j;
    configFile >> j;
    std::cout << "Email: " << j["Email"] << "\n";
    return j["Email"];
}

std::string Utility::getHostnameFromConfig() {
    std::ifstream configFile;
    configFile.open(std::filesystem::path(getenv("HOME")) / ".bongo/config.json");
    std::cout << "Reading config file ...\n";
    nlohmann::json j;
    configFile >> j;
    std::cout << "Hostname: " << j["Hostname"] << "\n";
    return j["Hostname"];
}

bool Utility::configExists() {
    std::cout << "Checking if config exists ...\n";
    if (std::filesystem::exists(std::string(getenv("HOME")) + "/.bongo/config.json")) {
        return true;
    } else {
        return false;
    }
}

bool Utility::isLoggedIn() {
    std::cout << "Checking if user is logged in ...\n";
    if (std::filesystem::exists(std::string(getenv("HOME")) + "/.bongo/config.json")) {
        std::ifstream configFile;
        configFile.open(std::string(getenv("HOME")) + "/.bongo/config.json");
        nlohmann::json j;
        configFile >> j;
        if (j["isLoggedIn"]) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Utility::LogIn() {
    std::cout << "Logging in ...\n";
    std::ifstream configFile;
    configFile.open(std::string(getenv("HOME")) + "/.bongo/config.json");
    nlohmann::json j;
    configFile >> j;
    j["isLoggedIn"] = true;
    std::ofstream o(std::string(getenv("HOME")) + "/.bongo/config.json");
    o << std::setw(4) << j << std::endl;

}

void Utility::LogOut() {
    std::cout << "Logging out ...\n";
    std::ifstream configFile;
    configFile.open(std::string(getenv("HOME")) + "/.bongo/config.json");
    nlohmann::json j;
    configFile >> j;
    j["isLoggedIn"] = false;
    std::ofstream o(std::string(getenv("HOME")) + "/.bongo/config.json");
    o << std::setw(4) << j << std::endl;
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

// TODO start here tomorrow
std::string Utility::getDefaultDirectory() {
    std::string hostname = getHostnameFromConfig();
    std::string bongoDirectory = std::filesystem::path(getenv("HOME")) / ".bongo";
    std::string directoryPath = bongoDirectory + "/" + hostname;
    std::cout << "Directory path: " << directoryPath << "\n";
    return directoryPath;
}