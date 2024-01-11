#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include "DatabaseManager.h"
#include "WorkspaceManager.h"

class UserManager {
public:
    UserManager();
    bool addUser(const std::string& email, const std::string& hostname, const std::string& passwordHash);
    bool deleteUser(const std::string& email);
    std::string getUser(const std::string& email);
    int getUserIdByEmail(const std::string& email);

private:
    DatabaseManager& dbManager;
};

#endif // USERMANAGER_H
