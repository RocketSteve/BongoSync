#include "../../include/Database/WorkspaceManager.h"

WorkspaceManager::WorkspaceManager() : dbManager(DatabaseManager::getInstance()) {}


bool WorkspaceManager::createWorkspace(const std::string& ownerEmail, const std::string& workspaceHash) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time_t);
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    std::string timeStr = ss.str();

    std::vector<std::string> values = {ownerEmail, workspaceHash, timeStr};
    std::vector<std::string> columns = {"owner_email", "workspace_hash", "modified_at"};
    return dbManager.createRecord("workspace", columns, values);
}

std::string WorkspaceManager::readWorkspaceByEmail(const std::string& ownerEmail) {
    std::string query = "SELECT * FROM WORKSPACE WHERE owner_email = $1;";
    return dbManager.readCustomRecord(query, {ownerEmail});
}

bool WorkspaceManager::updateWorkspaceByEmail(const std::string& ownerEmail, const std::string& newWorkspaceHash) {
    try {
        int workspaceId = getWorkspaceIdByEmail(ownerEmail);
        if (workspaceId == -1) {
            std::cerr << "Workspace not found for email: " << ownerEmail << std::endl;
            return false;
        }

        // Get the current timestamp
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        std::stringstream ss;
        ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        std::string currentTimestamp = ss.str();

        // Update the workspace hash
        if (!dbManager.updateRecord("WORKSPACE", "workspace_hash", "workspace_id", workspaceId, newWorkspaceHash)) {
            return false;
        }

        return dbManager.updateRecord("WORKSPACE", "modified_at", "workspace_id", workspaceId, currentTimestamp);
    } catch (const std::exception& e) {
        std::cerr << "Error updating workspace: " << e.what() << std::endl;
        return false;
    }
}


bool WorkspaceManager::deleteWorkspaceByEmail(const std::string& ownerEmail) {
    std::string query = "DELETE FROM WORKSPACE WHERE owner_email = $1;";
    return dbManager.deleteRecord(query, getWorkspaceIdByEmail(ownerEmail));
}

int WorkspaceManager::getWorkspaceIdByEmail(const std::string& ownerEmail) {
    try {
        // SQL query to get workspace ID
        std::string query = "SELECT workspace_id FROM WORKSPACE WHERE owner_email = $1;";

        // Execute the query and get the result as a string
        std::string workspaceIdStr = dbManager.readCustomRecord(query, {ownerEmail});

        // Check if the result is not empty and convert to int
        return !workspaceIdStr.empty() ? std::stoi(workspaceIdStr) : -1;
    } catch (const std::exception& e) {
        std::cerr << "Error getting workspace ID: " << e.what() << std::endl;
        return -1;
    }
}

std::string WorkspaceManager::getModifiedAtByUserEmail(const std::string& userEmail) {
    std::string query = "SELECT modified_at FROM workspace WHERE owner_email = $1;";
    std::vector<std::string> values = {userEmail};

    return DatabaseManager::getInstance().readCustomRecord(query, values);
}
