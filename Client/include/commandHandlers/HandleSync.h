#ifndef CLIENT_HANDLESYNC_H
#define CLIENT_HANDLESYNC_H

#include "../Utility.h"
#include "../communication/ServerCommunicator.h"
#include "../communication/MessageBuilder.h"
#include "../communication/MessageCreator.h"
#include "../fileServices/MerkleTree.h"
#include "../fileServices/ConflictResolver.h"
#include "../fileServices/FileWatcher.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>

class HandleSync {
public:
    HandleSync(const std::string& directoryPath);
    void initiateSync();
    void onFileChangeDetected();
    void synchronizeFiles();
    void handleServerResponse();

private:
    MerkleTree merkleTree;
    ConflictResolver conflictResolver;

    bool checkWithServer(const std::string& currentHash, const std::string& hostname, const std::string& modifiedAt);
    void processServerSyncResponse(const std::string& response);
    void handleLocalChanges();
    void startFileWatcherIfNeeded();

    void sendDeleteMessageToServer(const std::string& filePath);
    void sendFileToServer(const std::string& filePath);

    void listenForUpdates();

    void handleRemoteUpdate(const nlohmann::json& updateInfo);
    std::string directoryPath;

    bool transferMerkleTreeToServer();
};

#endif //CLIENT_HANDLESYNC_H
