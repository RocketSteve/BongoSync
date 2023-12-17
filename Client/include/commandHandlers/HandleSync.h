#ifndef CLIENT_HANDLESYNC_H
#define CLIENT_HANDLESYNC_H

#include "../Utility.h"
#include "../communication/ServerCommunicator.h"
#include "../communication/MessageBuilder.h"
#include "../communication/MessageCreator.h"
#include "../fileServices/MerkleTree.h"
#include "../fileServices/FileChangeDetector.h"
#include "../fileServices/ConflictResolver.h"
#include "../fileServices/FileWatcher.h"
#include <iostream>
#include <vector>

class HandleSync {
public:
    HandleSync(const std::string& directoryPath);
    void initiateSync();
    void onFileChangeDetected();
    void synchronizeFiles();

private:
    MerkleTree merkleTree;
    FileChangeDetector fileChangeDetector;
    ConflictResolver conflictResolver;

    bool checkWithServer(const std::string& currentHash, const std::string& hostname);
    void processServerSyncResponse(const std::string& response);
    void handleLocalChanges();
    void startFileWatcherIfNeeded();

    void sendChangedFilesToServer(const std::vector<FileChange>& changes);
    void sendFileToServer(const std::string& filePath);

    void handleConflict(const std::string& conflictedFile);
    std::string directoryPath;
};

#endif //CLIENT_HANDLESYNC_H
