#include "../../include/fileServices/FileChangeDetector.h"


FileChange::FileChange(std::string filePath, Type changeType) : filePath(std::move(filePath)), changeType(changeType) {}

std::string FileChange::getFilePath() const {
    return filePath;
}

FileChange::Type FileChange::getChangeType() const {
    return changeType;
}

FileChangeDetector::FileChangeDetector(const std::string& directoryPath) : directoryPath(directoryPath) {
    buildInitialTree();
}

void FileChangeDetector::buildInitialTree() {
    oldMerkleTree.buildTree(directoryPath);
}

void FileChangeDetector::rebuildTree() {
    newMerkleTree.buildTree(directoryPath);
}

std::vector<FileChange> FileChangeDetector::detectChanges() {
    rebuildTree();
    return compareTrees();
}

std::vector<FileChange> FileChangeDetector::compareTrees() {
    std::vector<FileChange> changes;
    std::stack<std::pair<MerkleTree::Node*, MerkleTree::Node*>> stack;

    stack.push({oldMerkleTree.getRoot(), newMerkleTree.getRoot()});

    while (!stack.empty()) {
        auto [oldNode, newNode] = stack.top();
        stack.pop();

        if (!oldNode && !newNode) {
            continue;
        }

        if (!oldNode || !newNode || oldNode->hash != newNode->hash) {
            if (oldNode && !newNode) {
                changes.push_back(FileChange(oldNode->filePath, FileChange::Type::Deleted));
            } else if (!oldNode && newNode) {
                changes.push_back(FileChange(newNode->filePath, FileChange::Type::Added));
            } else {
                changes.push_back(FileChange(newNode->filePath, FileChange::Type::Modified));
            }
            break;
        }

        stack.push({oldNode->left.get(), newNode->left.get()});
        stack.push({oldNode->right.get(), newNode->right.get()});
    }

    return changes;
}

// Implement other helper methods as needed.
