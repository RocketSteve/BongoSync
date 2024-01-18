#include "../../include/Database/SharedManager.h"
#include <iostream>

SharedManager::SharedManager() : dbManager(DatabaseManager::getInstance()) {
    // Initialize any required state
}

bool SharedManager::addSharedWorkspace(const std::string& ownerEmail, const std::string& sharedWithEmail) {
    int workspaceId = getWorkspaceIdByEmail(ownerEmail);
    if (workspaceId == -1) {
        std::cerr << "Workspace ID not found for email: " << ownerEmail << std::endl;
        return false;
    }

    std::vector<std::string> values = {std::to_string(workspaceId), sharedWithEmail};
    std::vector<std::string> columns = {"workspace_id", "shared_with_email"};

    return dbManager.createRecord("workspaceshared", columns, values);
}


bool SharedManager::deleteSharedWorkspace(const std::string& ownerEmail, const std::string& sharedWithEmail) {
    int workspaceId = getWorkspaceIdByEmail(ownerEmail);
    if (workspaceId == -1) {
        std::cerr << "Workspace ID not found for email: " << ownerEmail << std::endl;
        return false;
    }

    std::string query = "DELETE FROM WORKSPACESHARED WHERE workspace_id = $1 AND shared_with_email = $2;";
    std::vector<std::string> values = {std::to_string(workspaceId), sharedWithEmail};

    return dbManager.deleteRecord("WORKSPACESHARED", workspaceId);
}



std::vector<std::string> SharedManager::getSharedWorkspaces(const std::string& ownerEmail) {
    int workspaceId = getWorkspaceIdByEmail(ownerEmail);
    if (workspaceId == -1) {
        std::cerr << "Workspace ID not found for email: " << ownerEmail << std::endl;
        return {};
    }

    std::string query = "SELECT shared_with_email FROM WORKSPACESHARED WHERE workspace_id = $1;";
    std::vector<std::string> values = {std::to_string(workspaceId)};

    std::string result = dbManager.readCustomRecord(query, values);

    std::vector<std::string> sharedWithEmails;
    std::stringstream ss(result);
    std::string email;
    while (ss >> email) {
        sharedWithEmails.push_back(email);
    }

    return sharedWithEmails;
}


int SharedManager::getWorkspaceIdByEmail(const std::string& ownerEmail) {
    std::string query = "SELECT workspace_id FROM WORKSPACE WHERE owner_email = $1;";
    std::vector<std::string> values = {ownerEmail};

    std::string result = dbManager.readCustomRecord(query, values);

    try {
        return std::stoi(result);
    } catch (const std::exception& e) {
        std::cerr << "Error converting workspace ID: " << e.what() << std::endl;
        return -1;
    }
}




// Implement other methods as needed
