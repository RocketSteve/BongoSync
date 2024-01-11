#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include "DatabaseManager.h"
#include <string>

class FilesManager {
public:
    FilesManager();

    bool addFile(const std::string& workspaceId, const std::string& fileHash, const std::string& filePath);
    bool updateFile(int fileId, const std::string& newFileHash, const std::string& newFilePath);
    bool deleteFile(const std::string& filePath);
    std::string readFile(int fileId);
    int getWorkspaceIdByOwnerEmail(const std::string& ownerEmail);
    int getFileIdByPath(const std::string& filePath);

private:
    DatabaseManager& dbManager;
};

#endif // FILESMANAGER_H
