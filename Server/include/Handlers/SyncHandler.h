#ifndef SYNCHANDLER_H
#define SYNCHANDLER_H

#include <string>
#include <chrono>
#include <sstream>
#include "../Database/DatabaseManager.h"
#include "../Database/WorkspaceManager.h"
#include "../FileServices/FileChangeDetector.h"


class SyncHandler {
public:
    static bool isLatestVersion(const std::string& treeHash, const std::string& ownerEmail, const std::string& modifiedAt);
    static std::string compareTimestamps(const std::string& userEmail, const std::string& clientModifiedAt);
    static void treeReception(bool ahead, const std::string& userEmail);
    static std::string getDirectoryPath(const std::string& userEmail);
};

#endif // SYNCHANDLER_H
