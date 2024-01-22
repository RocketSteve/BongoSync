#include "../../include/FileServices/MerkleTreeComparer.h"
#include <filesystem>
#include <iostream>

MerkleTreeComparer::MerkleTreeComparer(const MerkleTree& baseTree, const MerkleTree& currentTree)
        : baseTree(baseTree), currentTree(currentTree) {
}

void MerkleTreeComparer::compare() {
    compareTrees(baseTree.getRoot(), currentTree.getRoot());
}

std::vector<std::string> MerkleTreeComparer::getAddedNodes() const {
    return addedNodes;
}

std::vector<std::string> MerkleTreeComparer::getRemovedNodes() const {
    return removedNodes;
}

std::vector<std::string> MerkleTreeComparer::getModifiedNodes() const {
    return modifiedNodes;
}

void MerkleTreeComparer::compareTrees(std::shared_ptr<MerkleTree::Node> baseNode, std::shared_ptr<MerkleTree::Node> currentNode) {
    // Base case: if both nodes are null, return
    if (!baseNode && !currentNode) {
        return;
    }

    // If the base node is null (current node is added)
    if (!baseNode) {
        addedNodes.push_back(currentNode->path);
        // Recursively check all children of the added node
        for (const auto& child : currentNode->children) {
            compareTrees(nullptr, child);
        }
        return;
    }

    // If the current node is null (base node is removed)
    if (!currentNode) {
        removedNodes.push_back(baseNode->path);
        // Recursively check all children of the removed node
        for (const auto& child : baseNode->children) {
            compareTrees(child, nullptr);
        }
        return;
    }

    // If both nodes are present but hashes are different (modified node)
    if (baseNode->hash != currentNode->hash && currentNode->isFile) {
        std::string baseFileContent = HashCalculator::getFileContent(baseNode->path);
        std::string currentFileContent = HashCalculator::getFileContent(currentNode->path);
        if (baseFileContent != currentFileContent) {
            modifiedNodes.push_back(currentNode->path);
        }
    }

    // Create maps for children of both nodes for efficient lookup
    std::map<std::string, std::shared_ptr<MerkleTree::Node>> baseChildren;
    std::map<std::string, std::shared_ptr<MerkleTree::Node>> currentChildren;

    for (const auto& child : baseNode->children) {
        baseChildren[child->path] = child;
    }

    for (const auto& child : currentNode->children) {
        currentChildren[child->path] = child;
    }

    // Compare children of the nodes
    for (const auto& [path, childNode] : currentChildren) {
        auto baseChildIter = baseChildren.find(path);
        if (baseChildIter != baseChildren.end()) {
            // Node exists in both trees, compare them recursively
            compareTrees(baseChildIter->second, childNode);
            baseChildren.erase(baseChildIter); // Remove the node from baseChildren as it's already processed
        } else {
            // Node is added in the current tree
            compareTrees(nullptr, childNode);
        }
    }

    // Any remaining nodes in baseChildren are removed in the current tree
    for (const auto& [path, childNode] : baseChildren) {
        compareTrees(childNode, nullptr);
    }
}