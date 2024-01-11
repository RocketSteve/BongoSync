#ifndef REGISTRATIONHANDLER_H
#define REGISTRATIONHANDLER_H

#include <string>
#include "Database/UserManager.h"

class RegistrationHandler {
public:
    static bool handleRegistration(const std::string& email, const std::string& password, const std::string& hostname);
};

#endif // REGISTRATIONHANDLER_H
