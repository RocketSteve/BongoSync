#include "../include/CollisionDetector.h"

// TODO Refactor to new class
// TODO Hash calculator and persistance manager

CollisionDetector::CollisionDetector(const std::string& rootDir)
        : rootDirectory(rootDir), rootNode("") {}

void CollisionDetector::buildTree() {
    rootNode = buildTreeForDirectory(rootDirectory);
}

MerkleNode CollisionDetector::buildTreeForDirectory(const std::string& directory) {
    MerkleNode node("");
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_directory()) {
            node.children.push_back(buildTreeForDirectory(entry.path()));
        } else if (entry.is_regular_file()) {
            std::string fileHash = HashCalculator::calculateHash(entry.path());
            node.children.push_back(MerkleNode(fileHash));
        }
    }
    node.hash = calculateDirectoryHash(node);
    return node;
}

std::string CollisionDetector::calculateDirectoryHash(const MerkleNode& node) {
    std::string combinedHash;
    for (const auto& child : node.children) {
        combinedHash += child.hash;
    }
    return HashCalculator::calculateHash(combinedHash);
}

void CollisionDetector::handleFileChange(const std::string& filename, int events) {
    // Handle file changes and update the Merkle tree accordingly
    // This implementation depends on how you want to handle file changes
}
