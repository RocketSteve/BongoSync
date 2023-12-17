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



class Utility {
public:
    static std::string promptForEmail();
    static std::string promptForPassword();
    static std::string getHostname();
    static std::string getEmailFromConfig();
    static std::string hashPassword(const std::string& password);
    static std::string getTreeHash();
    static bool confirmPassword(const std::string& password);
    static bool configExists();
    static bool isLoggedIn();
    static void LogIn();
};

#endif //CLIENT_UTILITY_H
