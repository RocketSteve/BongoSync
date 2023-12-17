#ifndef FILESERVICES_H
#define FILESERVICES_H

#include "FileWatcher.h"
#include "ConflictResolver.h"
#include "MerkleTree.h"
#include "../communication/ServerCommunicator.h"

class FileServices {
public:
    FileServices(const std::string& directoryPath, ServerCommunicator& serverCommunicator);
    void start();
    void stop();

private:
    FileWatcher fileWatcher;
    ConflictResolver conflictResolver;
    MerkleTree merkleTree;
    ServerCommunicator& serverCommunicator;

    void onFileChange(const std::string& filePath);
    void handleConflicts(const std::vector<FileChange>& changes);
};

#endif // FILESERVICES_H
