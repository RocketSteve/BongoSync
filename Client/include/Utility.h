#ifndef CLIENT_UTILITY_H
#define CLIENT_UTILITY_H

#include <string>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <bcrypt/BCrypt.hpp>


class Utility {
public:
    static std::string promptForEmail();
    static std::string promptForPassword();
    static std::string getHostname();
    static std::string getEmailFromConfig();
    static std::string hashPassword(const std::string& password);
    static bool confirmPassword(const std::string& password);
};

#endif //CLIENT_UTILITY_H
