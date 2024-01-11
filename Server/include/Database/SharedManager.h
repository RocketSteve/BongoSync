#ifndef SHAREDMANAGER_H
#define SHAREDMANAGER_H

#include "DatabaseManager.h"
#include <string>
#include <vector>

class SharedManager {
public:
    SharedManager();

    bool addSharedWorkspace(const std::string& ownerEmail, const std::string& sharedWithEmail);
    bool deleteSharedWorkspace(const std::string& ownerEmail, const std::string& sharedWithEmail);
    std::vector<std::string> getSharedWorkspaces(const std::string& ownerEmail);
    int getWorkspaceIdByEmail(const std::string& ownerEmail);

private:
    DatabaseManager& dbManager;
};

#endif // SHAREDMANAGER_H
