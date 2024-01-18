#ifndef CLIENT_UTILITY_H
#define CLIENT_UTILITY_H

#include <string>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <arpa/inet.h>



class Utility {
public:
    static std::string promptForEmail();
    static std::string promptForPassword();
    static std::string getHostname();
    static std::string getEmailFromConfig();
    static std::string hashPassword(const std::string& password);
    static std::string getDefaultDirectory();
    static std::string getHostnameFromConfig();
    static std::string getPathFromConfig();
    static std::string readModifiedAt();
    static void updateWorkspaceHash(const std::string& newWorkspaceHash);
    static std::string readWorkspaceHash();
    static std::pair<std::string, int>  readServerConfig();

    static bool confirmPassword(const std::string& password);
    static bool configExists();

    static bool isValidIPAddress(const std::string& ipAddress);
    static bool isValidPort(int port);

    static std::string getConfigFilePath();
    static std::string getBongoDir();

private:

    static nlohmann::json readConfigFile();
    static void writeConfigFile(const nlohmann::json& config);
    static std::string getCurrentTimestamp();
};

#endif //CLIENT_UTILITY_H
