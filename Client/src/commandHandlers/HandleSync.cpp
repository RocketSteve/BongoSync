#include "../../include/commandHandlers/HandleSync.h"
#include <nlohmann/json.hpp>

HandleSync::HandleSync(const std::string& directoryPath)
        : directoryPath(directoryPath),
          merkleTree(),
          fileChangeDetector(directoryPath),
          conflictResolver(ServerCommunicator::getInstance()) {}

void HandleSync::initiateSync() {
    if (!Utility::isLoggedIn()) {
        std::cout << "Not logged in, please start the application\n";
        return;
    }

    merkleTree.buildTree(directoryPath); // Build or update Merkle Tree
    std::string currentHash = merkleTree.getTreeHash();
    std::string hostname = Utility::getHostname();

    if (checkWithServer(currentHash, hostname)) {
        std::cout << "Synchronization complete.\n";
    } else {
        std::cout << "Error during synchronization.\n";
    }
}

bool HandleSync::checkWithServer(const std::string& currentHash, const std::string& hostname) {
    std::string syncRequestMsg = MessageCreator::create()
            .setHostname(hostname)
            .setTreeHash(currentHash)
            .buildAskIfLatestMessage();

    auto& serverCommunicator = ServerCommunicator::getInstance();
    if (!serverCommunicator.isConnectedToServer() && !serverCommunicator.connectToServer()) {
        std::cerr << "Failed to connect to server for synchronization.\n";
        return false;
    }

    if (!serverCommunicator.sendMessage(syncRequestMsg)) {
        std::cerr << "Failed to send synchronization request.\n";
        return false;
    }

    std::string serverResponse;
    if (!serverCommunicator.receiveMessage(serverResponse)) {
        std::cerr << "Failed to receive response from server.\n";
        return false;
    }

    processServerSyncResponse(serverResponse);
    return true;
}

void HandleSync::processServerSyncResponse(const std::string& response) {
    nlohmann::json responseJson = nlohmann::json::parse(response);

    if (responseJson.contains("status")) {
        std::string status = responseJson["status"];
        if (status == "SYNC_NEEDED") {
            handleLocalChanges();
        } else if (status == "NO_SYNC_REQUIRED") {
            std::cout << "Local copy is up to date.\n";
            startFileWatcherIfNeeded();
        } else if (status == "REMOTE_UPDATE") {
            handleRemoteUpdate(responseJson); // Handle the server's update notification
        } else {
            std::cerr << "Unknown status received from server: " << status << "\n";
        }
    } else {
        std::cerr << "Invalid response format received from server.\n";
    }
}

void HandleSync::handleLocalChanges() {
    auto changes = fileChangeDetector.detectChanges();
    if (!changes.empty()) {
        sendChangedFilesToServer(changes);
    }
}

void HandleSync::sendChangedFilesToServer(const std::vector<FileChange>& changes) {
    for (const auto& change : changes) {
        switch (change.getChangeType()) {
            case FileChange::Type::Added:
            case FileChange::Type::Modified:
                sendFileToServer(change.getFilePath());
                break;
            case FileChange::Type::Deleted:
                sendDeleteMessageToServer(change.getFilePath());
                break;
        }
    }
}

void HandleSync::sendFileToServer(const std::string& filePath) {
    ServerCommunicator& communicator = ServerCommunicator::getInstance();
    if (!communicator.sendFile(filePath)) {
        std::cerr << "Failed to send file: " << filePath << "\n";
    }
}

void HandleSync::sendDeleteMessageToServer(const std::string& filePath) {

    std::string deleteMessage = MessageCreator::create()
            .setFilePath(filePath)
            .buildDeleteMessage();

    // Send the delete message to the server
    ServerCommunicator& communicator = ServerCommunicator::getInstance();
    if (!communicator.sendMessage(deleteMessage)) {
        std::cerr << "Failed to send delete message for file: " << filePath << "\n";
    }
}


void HandleSync::handleRemoteUpdate(const nlohmann::json& updateInfo) {
    // Assuming updateInfo contains the remote Merkle Tree or a list of changed files
    // For simplicity, let's assume it's a list of file paths that have changed
    std::vector<std::string> remoteChangedFiles = updateInfo["changed_files"];

    merkleTree.buildTree(directoryPath);
    auto localChanges = fileChangeDetector.detectChanges();

    for (const auto& remoteFile : remoteChangedFiles) {
        bool isConflict = std::any_of(localChanges.begin(), localChanges.end(),
                                      [&remoteFile](const FileChange& change) {
                                          return change.getFilePath() == remoteFile;
                                      });

        if (isConflict) {
            conflictResolver.resolveConflict(remoteFile);
        } else {
            serverCommunicator.receiveFile(remoteFile); // Download file from server
        }
    }
}

void HandleSync::startFileWatcherIfNeeded() {
    FileWatcher& watcher = FileWatcher::getInstance();
    watcher.start();
}

// ... Other methods ...
