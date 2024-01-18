#include "../../include/Handlers/RegistrationHandler.h"


bool RegistrationHandler::handleRegistration(const std::string& email, const std::string& password, const std::string& hostname, const std::string& workspaceHash) {
    UserManager userManager;
    std::string usr = userManager.getUser(email);
    if (!usr.empty()) {
        return false;
    } else {
        // Add user to the database
        userManager.addUser(email, hostname ,password, workspaceHash);
        return true; // User added successfully
    }
}
