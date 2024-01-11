#include "../include/Database/UserManager.h"

UserManager::UserManager() : dbManager(DatabaseManager::getInstance()) {}


bool UserManager::addUser(const std::string& email, const std::string& hostname, const std::string& passwordHash) {
    try {
        WorkspaceManager workspaceManager;
        if (!workspaceManager.createWorkspace(email, "")) {
            return false;
        }
        std::string workspaceId = std::to_string(workspaceManager.getWorkspaceIdByEmail(email));

        std::vector<std::string> values = {email, hostname, passwordHash, workspaceId};
        std::vector<std::string> columns = {"email", "hostname", "password_hash", "workspace_id"};

        // Execute the query using DatabaseManager
        return dbManager.createRecord("app_users", columns, values);
    } catch (const std::exception& e) {
        std::cerr << "Error adding user: " << e.what() << std::endl;
        return false;
    }
}

bool UserManager::deleteUser(const std::string& email) {
    try {
        // Get the user ID associated with the email
        int userId = getUserIdByEmail(email);

        // Check if a valid user ID was found
        if (userId != -1) {
            // Use the retrieved ID
            return dbManager.deleteRecord("app_users", userId);
        } else {
            std::cerr << "User with the given email does not exist." << std::endl;
            return false; // User ID not found
        }
    } catch (const std::exception& e) {
        std::cerr << "Error deleting user: " << e.what() << std::endl;
        return false;
    }
}

std::string UserManager::getUser(const std::string& email) {
    try {
        // Get the user ID associated with the email
        int userId = getUserIdByEmail(email);

        // If user ID is valid, fetch user details
        if (userId != -1) {
            // Use the retrieved ID
            return dbManager.readRecord("app_users", userId);
        } else {
            return ""; // Return an empty string if user ID not found
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting user: " << e.what() << std::endl;
        return "";
    }
}


int UserManager::getUserIdByEmail(const std::string& email) {
    try {
        // Format and sanitize the input
        std::vector<std::string> values = {email};

        // Construct the SQL query
        std::string query = "SELECT user_id FROM app_users WHERE email = $1;";

        // Execute the query and get the result
        std::string userIdStr = dbManager.readCustomRecord(query, values);

        // Convert the string result to int
        return userIdStr.empty() ? -1 : std::stoi(userIdStr);
    } catch (const std::exception& e) {
        std::cerr << "Error getting user ID: " << e.what() << std::endl;
        return -1;
    }
}


