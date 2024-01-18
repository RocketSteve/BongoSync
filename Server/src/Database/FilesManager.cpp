#include "../../include/Database/FilesManager.h"

FilesManager::FilesManager() : dbManager(DatabaseManager::getInstance()) {}

bool FilesManager::addFile(const std::string& ownerEmail, const std::string& fileHash, const std::string& filePath) {
    int workspaceId = getWorkspaceIdByOwnerEmail(ownerEmail);
    if (workspaceId == -1) {
        std::cerr << "Workspace ID not found for email: " << ownerEmail << std::endl;
        return false;
    }

    // Generate the current timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time_t);
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    std::string currentTimestamp = ss.str();

    std::vector<std::string> values = {std::to_string(workspaceId), fileHash, filePath, currentTimestamp};
        std::vector<std::string> columns = {"workspace_id", "file_hash", "file_path", "modified_at"};

    return dbManager.createRecord("files", columns,  values);
}

bool FilesManager::deleteFile(const std::string& filePath) {
    try {
        int fileId = getFileIdByPath(filePath);
        if (fileId == -1) {
            std::cerr << "File ID not found for path: " << filePath << std::endl;
            return false;
        }

        // Assuming "FILES" is the table name in your database
        return dbManager.deleteRecord("FILES", fileId);
    } catch (const std::exception& e) {
        std::cerr << "Error deleting file: " << e.what() << std::endl;
        return false;
    }
}



std::string FilesManager::readFile(int fileId) {
    try {
        std::string query = "SELECT * FROM FILES WHERE file_id = $1;";
        std::vector<std::string> values = {std::to_string(fileId)};
        return dbManager.readCustomRecord(query, values);
    } catch (const std::exception& e) {
        std::cerr << "Error reading file: " << e.what() << std::endl;
        return "";
    }
}

int FilesManager::getWorkspaceIdByOwnerEmail(const std::string& ownerEmail) {
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


int FilesManager::getFileIdByPath(const std::string& filePath) {
    std::string query = "SELECT file_id FROM FILES WHERE file_path = $1;";
    std::vector<std::string> values = {filePath};
    std::string fileIdStr = dbManager.readCustomRecord(query, values);
    return fileIdStr.empty() ? -1 : std::stoi(fileIdStr);
}