#include "../include/HandleSync.h"

void HandleSync::initiateSync() {

    std::string hostname = Utility::getHostname();
    std::string treeHash = Utility::getTreeHash();

    if (checkWithServer(treeHash, hostname)) {
        std::cout << "User does not exist, please try again.\n";
        initiateSync();
    } else {
        std::cout << "Syncing ...\n";
        std::cout << "Sync complete.\n";
    }
}

bool HandleSync::checkWithServer(std::string& treeHash, std::string& hostname) {
    // Create a message to request synchronization data using MessageBuilder
    std::string syncRequestMsg = MessageCreator::create()
            .setTreeHash(treeHash)
            .setHostname(hostname)
            .buildAskIfLatestMessage();

    // Get the instance of ServerCommunicator
    auto& serverCommunicator = ServerCommunicator::getInstance();
    if (!serverCommunicator.isConnectedToServer() && !serverCommunicator.connectToServer()) {
        std::cerr << "Failed to connect to server for synchronization.\n";
        return 0;
    }

    // Send the synchronization request message
    if (!serverCommunicator.sendMessage(syncRequestMsg)) {
        std::cerr << "Failed to send synchronization request.\n";
        return 0;
    }

    // Example: Receive response from the server
    std::string serverResponse;
    if (!serverCommunicator.receiveMessage(serverResponse)) {
        std::cerr << "Failed to receive response from server.\n";
        return 0;
    }

    // Process the server's response
    // Implementation depends on your server's protocol and data format
    processServerSyncResponse(serverResponse);
    return 1;
}

void HandleSync::processServerSyncResponse(const std::string& response) {
    // Example processing (you'll need to replace this with actual logic)
    if (response == "SYNC_NEEDED") {
        // Perform synchronization actions
    } else if (response == "NO_SYNC_REQUIRED") {
        // No action needed
    } else {
        // Handle other cases or errors
    }
}