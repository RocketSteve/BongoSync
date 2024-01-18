#include "../../include/commandHandlers/HandleSync.h"
#include <nlohmann/json.hpp>

// TODO add message interpretation
//Client:
//Calcualtre hash and send it to server
//Wait for response
//Server:
//Compare hashes
//Send list of changed files
//Client:
//receive changes
//resolve conflicts use lsof | grep

//bongo sync:
//- calculate local dir hash, and current time
//- server compares client hash to hash+last modification time thats known to the server
//- if server is ahead -
//transfer diffing files to client - upon completion client writes last received hash
//- if client is ahead -
//send tree hash and time to server, server once receives the files,
//updates the tree hash (do you need "server busy on this workspace" so two db writes dont come in at the same time?)


//File watcher is only supposed to trigger initiate sync.
//Initiate sync will take care of:
//- checking with server if synchronization is needed
//- if needed, send files to server, if it is needed the mechanism should:
//1. check what version is latest
//1.1 if server is version is newer then receive files from server.
//this will download newer files and mark conflicts
//if there will be still files left to sync then (local changes not reflected on server and not synced because of server priority)
//then the sync should be called again to send the files to server (local hash will have later date of modification so it will
//tree for comparison and then server will detect that there are files that it doesn't have and will request these)
//1.2 if client version is newer then send files to server
//- if not needed, do nothing
//on receival of files handle sync will check if there are any files opened of the same name as the received ones
//if opened it will rename files from the server.
//if not opened it will just save the files.

//client shoudl only have one instance of handle sync

HandleSync::HandleSync(const std::string& directoryPath)
        : directoryPath(directoryPath),
          merkleTree(),
          conflictResolver(ServerCommunicator::getInstance()) {}

void HandleSync::initiateSync() {

    merkleTree.buildTree(directoryPath);
    std::string currentHash = merkleTree.getTreeHash();
    std::string hostname = Utility::getHostname();
    std::string modifiedAt = Utility::readModifiedAt();

    if (checkWithServer(currentHash, hostname, modifiedAt)) {
        std::cout << "Synchronization complete.\n";
    } else {
        std::cout << "Error during synchronization.\n";
    }

    std::thread updateListenerThread(&HandleSync::listenForUpdates, this);
    updateListenerThread.detach();
}

bool HandleSync::checkWithServer(const std::string& currentHash, const std::string& hostname, const std::string& modifiedAt) {
    std::cout << "Checking with server ...\n" << hostname << "\n";
    std::string syncRequestMsg = MessageCreator::create()
            .setHostname(hostname)
            .setTreeHash(currentHash)
            .setModifiedAt(modifiedAt)
            .buildAskIfLatestMessage();

    std::cout << "Sync request message: " << syncRequestMsg << "\n";

    auto& serverCommunicator = ServerCommunicator::getInstance();

    if (!serverCommunicator.sendMessage(syncRequestMsg)) {
        std::cerr << "Failed to send synchronization request.\n";
        return false;
    }

    // Receive the response
    std::string responseStr = serverCommunicator.receiveMessage();
    if (responseStr.empty()) {
        std::cerr << "Failed to receive response from server.\n";
        return false;
    }

    // Handle the response
    try {
        auto responseJson = nlohmann::json::parse(responseStr);
        std::string responseType = responseJson.value("response", "");

        if (responseType == "latest") {
            std::cout << "Server response: Latest version.\n";
            return true;
        } else if (responseType == "not_latest") {
            std::cout << "Server response: Not the latest version.\n";

            std::string ahead = "";
            if (responseJson.contains("data") && responseJson["data"].is_object()) {
                ahead = responseJson["data"].value("ahead", "");
            }

            std::cout << "ahead: " << ahead << "\n";

            MerkleTree tree;
            std::string directoryPath = Utility::getPathFromConfig();
            tree.buildTree(directoryPath);

            tree.serializeTree();

            if (!serverCommunicator.sendMerkleTreeFile(ahead)) {
                std::cerr << "Failed to transfer Merkle tree to server.\n";
                return false;
            } else {
                std::cout << "Merkle tree sent to server.\n";
            }



            return true;
        } else {
            std::cerr << "Unexpected server response.\n";
            return false;
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
        return false;
    }
}

void HandleSync::sendFileToServer(const std::string& filePath) {
    ServerCommunicator& communicator = ServerCommunicator::getInstance();
    if (!communicator.sendFile(filePath)) {
        std::cerr << "Failed to send file: " << filePath << "\n";
    }
}

void HandleSync::listenForUpdates() {
    ServerCommunicator& serverCommunicator = ServerCommunicator::getInstance();

    while (true) {
        if (serverCommunicator.isMessageReady()) {  // Check if a message is ready
            std::string responseStr = serverCommunicator.receiveMessage();
            if (!responseStr.empty()) {
                try {
                    auto responseJson = nlohmann::json::parse(responseStr);
                    bool isSuccess = responseJson.value("success", false);
                    std::string message = responseJson.value("message", "");

                    std::cout << "Server response: " << message << "\n";
                } catch (const nlohmann::json::parse_error& e) {
                    std::cerr << "JSON parsing error: " << e.what() << "\n";
                }
            }
        } else {
            // Sleep for a short duration before checking again
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}



