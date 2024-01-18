#include "../include/Utility.h"


std::string Utility::promptForEmail() {
    std::string email;
    std::cout << "Enter your email: ";
    std::cin >> email;
    return email;
}

std::string Utility::getHostname() {
    return "files";
}



std::string Utility::getEmailFromConfig() {
    std::ifstream configFile;
    configFile.open(std::filesystem::path(getenv("HOME")) / ".bongo/config.json");
    std::cout << "Getting email config file ...\n";
    nlohmann::json j;
    configFile >> j;
    std::cout << "Email: " << j["Email"] << "\n";
    return j["Email"];
}

std::string Utility::getHostnameFromConfig() {
    std::ifstream configFile;
    configFile.open(std::filesystem::path(getenv("HOME")) / ".bongo/config.json");
    std::cout << "Getting hostname config file ...\n";
    nlohmann::json j;
    configFile >> j;
    std::cout << "Hostname: " << j["Hostname"] << "\n";
    return j["Hostname"];
}

std::string Utility::getPathFromConfig() {
    std::ifstream configFile;
    configFile.open(std::filesystem::path(getenv("HOME")) / ".bongo/config.json");
    std::cout << "Getting path from config file  ...\n";
    nlohmann::json j;
    configFile >> j;
    std::cout << "Path: " << j["Path"] << "\n";
    return j["Path"];
}


bool Utility::configExists() {
    std::cout << "Checking if config exists ...\n";
    if (std::filesystem::exists(std::string(getenv("HOME")) + "/.bongo/config.json")) {
        return true;
    } else {
        return false;
    }
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

std::string Utility::getDefaultDirectory() {
    std::string hostname = getHostnameFromConfig();
    std::string path = getPathFromConfig();
    std::string directoryPath = path;
    std::cout << "Directory path: " << directoryPath << "\n";
    return directoryPath;
}


// Helper method to construct config file path
std::string Utility::getConfigFilePath() {
    return (std::filesystem::path(getenv("HOME")) / ".bongo/config.json").string();
}

std::string Utility::getBongoDir() {
    return (std::filesystem::path(getenv("HOME")) / ".bongo").string();
}

// Helper method to read config file
nlohmann::json Utility::readConfigFile() {
    std::ifstream configFile(getConfigFilePath());
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to open config file: " + getConfigFilePath());
    }
    nlohmann::json configJson;
    configFile >> configJson;
    return configJson;
}

std::pair<std::string, int> Utility::readServerConfig() {
    nlohmann::json configJson = readConfigFile();
    std::string serverIP = configJson.value("ServerIP", "");
    int serverPort = configJson.value("ServerPort", 0);
    return {serverIP, serverPort};
}

// Helper method to write to config file
void Utility::writeConfigFile(const nlohmann::json& configJson) {
    std::ofstream configFile(getConfigFilePath());
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to open config file for writing: " + getConfigFilePath());
    }
    configFile << std::setw(4) << configJson;
}

// Helper method to get current timestamp
std::string Utility::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time_t);
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Utility::readWorkspaceHash() {
    nlohmann::json configJson = readConfigFile();
    return configJson.value("WorkspaceHash", "");
}

// Method to update workspace hash
void Utility::updateWorkspaceHash(const std::string& newWorkspaceHash) {
    try {
        nlohmann::json configJson = readConfigFile();
        configJson["WorkspaceHash"] = newWorkspaceHash;
        configJson["modified_at"] = getCurrentTimestamp();  // Update the timestamp here
        writeConfigFile(configJson);
        std::cout << "Workspace hash and modified_at updated successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error updating workspace hash: " << e.what() << std::endl;
    }
}


// Method to read modified_at
std::string Utility::readModifiedAt() {
    nlohmann::json configJson = readConfigFile();
    return configJson.value("modified_at", "");
}

bool Utility::isValidIPAddress(const std::string& ipAddress) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}

bool Utility::isValidPort(int port) {
    return port > 0 && port <= 65535;
}