#include "../../include/fileServices/FileServices.h"

FileServices::FileServices(const std::string& directoryPath, ServerCommunicator& serverCommunicator)
        : fileWatcher(directoryPath),
          conflictResolver(serverCommunicator),
          merkleTree(),
          serverCommunicator(serverCommunicator) {
    fileWatcher.setFileChangeCallback(std::bind(&FileServices::onFileChange, this, std::placeholders::_1));
}

void FileServices::start() {
    fileWatcher.start();
    // Initialize and build the initial Merkle Tree
    merkleTree.buildTree(fileWatcher.getDirectoryToWatch());
}

void FileServices::stop() {
    fileWatcher.stop();
}

void FileServices::onFileChange(const std::string& filePath) {
    // Rebuild Merkle Tree and get the changes
    merkleTree.buildTree(fileWatcher.getDirectoryToWatch());
    auto changes = merkleTree.compareTrees();

    // Handle conflicts if any
    handleConflicts(changes);

    // Additional logic for other types of changes, if needed
}

void FileServices::handleConflicts(const std::vector<FileChange>& changes) {
    for (const auto& change : changes) {
        if (change.getChangeType() == FileChange::Type::Modified) {
            conflictResolver.resolveConflict(change.getFilePath());
        }
        // Handle other types of changes if necessary
    }
}
