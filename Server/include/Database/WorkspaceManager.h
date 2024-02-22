#ifndef WORKSPACEMANAGER_H
#define WORKSPACEMANAGER_H

#include <string>
#include <iomanip>
#include <chrono>
#include "DatabaseManager.h"

class WorkspaceManager {
public:
    WorkspaceManager();

    bool createWorkspace(const std::string& ownerEmail, const std::string& workspaceHash);
    std::string readWorkspaceByEmail(const std::string& ownerEmail);
    bool updateWorkspaceByEmail(const std::string& ownerEmail, const std::string& newWorkspaceHash);
    bool deleteWorkspaceByEmail(const std::string& ownerEmail);
    int getWorkspaceIdByEmail(const std::string& ownerEmail);
    std::string getModifiedAtByUserEmail(const std::string& userEmail);

private:
    DatabaseManager& dbManager;
};

#endif // WORKSPACEMANAGER_H
