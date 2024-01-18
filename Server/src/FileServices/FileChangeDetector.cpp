#include "../../include/fileServices/FileChangeDetector.h"
#include <stack>
#include <unordered_set>
#include <iostream>

// Constructor for initializing with the current state of a directory
FileChangeDetector::FileChangeDetector(const MerkleTree& deserializedTree)
        : oldMerkleTree(deserializedTree) {
    buildNewTree();
}

// Method to build the new Merkle tree from the /files directory
void FileChangeDetector::buildNewTree() {
    std::string filesDirectory = "files/";  // Path to the files directory
    newMerkleTree.buildTree(filesDirectory);

    std::cout << "New tree:" << std::endl;

    newMerkleTree.printTree();
}

std::string FileChangeDetector::getRelativePath(const std::string& filePath) {
    // Define the key path segment that we want to find
    const std::string keyPathSegment = "files/";

    // Find the position of '/files/' in the filePath
    size_t pos = filePath.find(keyPathSegment);
    if (pos != std::string::npos) {
        // Extract the part of the path after '/files/'
        return filePath.substr(pos + keyPathSegment.length());
    }

    // If '/files/' is not found, return the original filePath or handle the error as needed
    return filePath;
}



// Method to detect file changes between the old and new Merkle trees
std::vector<FileChange> FileChangeDetector::detectChanges(bool useAsNewTree) {
    return compareTrees(useAsNewTree);
}

// Method to compare the old and new Merkle trees and identify changes
std::vector<FileChange> FileChangeDetector::compareTrees(bool compareForward) {
    std::vector<FileChange> changes;
    std::stack<std::pair<std::shared_ptr<MerkleTree::Node>, std::shared_ptr<MerkleTree::Node>>> stack;

    if (compareForward) {
        stack.push({oldMerkleTree.getRoot(), newMerkleTree.getRoot()});
    } else {
        stack.push({newMerkleTree.getRoot(), oldMerkleTree.getRoot()});
    }

    while (!stack.empty()) {
        auto [firstNode, secondNode] = stack.top();
        stack.pop();

        if (!firstNode && !secondNode) {
            continue;
        }

        if (!firstNode || !secondNode || firstNode->hash != secondNode->hash) {
            FileChange::Type changeType;
            if (!firstNode) {
                changeType = compareForward ? FileChange::Type::Added : FileChange::Type::Deleted;
            } else if (!secondNode) {
                changeType = compareForward ? FileChange::Type::Deleted : FileChange::Type::Added;
            } else {
                changeType = FileChange::Type::Modified;
            }

            std::string path = secondNode ? secondNode->path : firstNode->path;
            changes.push_back(FileChange(getRelativePath(path), changeType));
            continue;
        }

        std::unordered_set<std::string> firstNodePaths, secondNodePaths;
        for (const auto& child : firstNode->children) firstNodePaths.insert(child->path);
        for (const auto& child : secondNode->children) secondNodePaths.insert(child->path);

        for (const auto& child : firstNode->children) {
            if (secondNodePaths.count(child->path) == 0) {
                FileChange::Type changeType = compareForward ? FileChange::Type::Deleted : FileChange::Type::Added;
                changes.push_back(FileChange(getRelativePath(child->path), changeType));
            } else {
                stack.push({child, secondNode->findChildByPath(child->path)});
            }
        }

        for (const auto& child : secondNode->children) {
            if (firstNodePaths.count(child->path) == 0) {
                FileChange::Type changeType = compareForward ? FileChange::Type::Added : FileChange::Type::Deleted;
                changes.push_back(FileChange(getRelativePath(child->path), changeType));
            }
        }
    }
    return changes;
}



// Constructor for FileChange objects
FileChange::FileChange(std::string filePath, Type changeType)
        : filePath(std::move(filePath)), changeType(changeType) {}

// Method to get the file path of a change
std::string FileChange::getFilePath() const {
    return filePath;
}

// Method to get the change type
FileChange::Type FileChange::getChangeType() const {
    return changeType;
}