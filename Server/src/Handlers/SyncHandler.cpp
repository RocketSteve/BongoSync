//
// Created by Szymon Tomiak on 12/01/2024.
//
//Check date of hash modification
//if > clientdate
//send tree to client
//client requests needed files
//if < clientdate
//send tree to client
//wait for file transfer

#include "../../include/Handlers/SyncHandler.h"
#include "../../include/FileServices/MerkleTreeComparer.h"

bool SyncHandler::isLatestVersion(const std::string& treeHash, const std::string& ownerEmail, const std::string& modifiedAt) {
    try {
        auto& dbManager = DatabaseManager::getInstance();

        // Construct the query to fetch workspace hash
        std::string query = "SELECT workspace_hash FROM workspace WHERE owner_email = $1;";
        std::vector<std::string> values = {ownerEmail};

        std::string workspaceHash = dbManager.readCustomRecord(query, values);

        // Compare the hashes
        return treeHash == workspaceHash;
    } catch (const std::exception& e) {
        std::cerr << "Error in SyncHandler::isLatestVersion: " << e.what() << std::endl;
        return false;
    }
}

std::string SyncHandler::compareTimestamps(const std::string& userEmail, const std::string& clientModifiedAt) {
    WorkspaceManager workspaceManager;
    std::string serverModifiedAt = workspaceManager.getModifiedAtByUserEmail(userEmail);

    // Parse timestamps
    std::tm clientTm = {};
    std::tm serverTm = {};
    std::istringstream clientStream(clientModifiedAt);
    std::istringstream serverStream(serverModifiedAt);

    clientStream >> std::get_time(&clientTm, "%Y-%m-%d %H:%M:%S");
    serverStream >> std::get_time(&serverTm, "%Y-%m-%d %H:%M:%S");

    auto clientTime = std::chrono::system_clock::from_time_t(std::mktime(&clientTm));
    auto serverTime = std::chrono::system_clock::from_time_t(std::mktime(&serverTm));

    std::cout << "Client time: " << clientTime.time_since_epoch().count() << std::endl;
    std::cout << "Server time: " << serverTime.time_since_epoch().count() << std::endl;

    if (clientTime > serverTime) {
        return "client";
    } else {
        return "server";
    }
}

void SyncHandler::treeReception(bool ahead, const std::string& userEmail) {
    std::cout << "Receiving tree from " << "client" << "..." << std::endl;
    std::string directoryPath = getDirectoryPath(userEmail);
    std::cout << "Directory path: " << directoryPath << std::endl;
    // Load and deserialize the Merkle Tree from file
    MerkleTree merkleTree = MerkleTree::loadFromFile("tree/tree.json");

    // Load the current tree from the "files" directory
    MerkleTree currentTree;
    currentTree.buildTree(directoryPath);

    std::cout << "Current tree:" << std::endl;
    currentTree.printTree();
    std::cout << "Received tree:" << std::endl;
    merkleTree.printTree();
    // Instantiate MerkleTreeComparer with the correct order of trees
    MerkleTreeComparer treeComparer(ahead ? merkleTree : currentTree, ahead ? currentTree : merkleTree);

    // Perform the comparison
    treeComparer.compare();

    // Retrieve changes detected by the comparer
    auto addedNodes = treeComparer.getAddedNodes();
    auto removedNodes = treeComparer.getRemovedNodes();
    auto modifiedNodes = treeComparer.getModifiedNodes();

    // Print the changes
    std::cout << "Detected changes:" << std::endl;
    
    // Print added nodes
    for (const auto& node : addedNodes) {
        std::cout << " - Added: " << node << std::endl;
    }

    // Print removed nodes
    for (const auto& node : removedNodes) {
        std::cout << " - Deleted: " << node << std::endl;
    }

    // Print modified nodes
    for (const auto& node : modifiedNodes) {
        std::cout << " - Modified: " << node << std::endl;
    }
}

std::string SyncHandler::getDirectoryPath(const std::string& userEmail) {
    std::string query = "SELECT hostname FROM app_users WHERE email = $1;";
    std::vector<std::string> values = {userEmail};

    return DatabaseManager::getInstance().readCustomRecord(query, values);
}