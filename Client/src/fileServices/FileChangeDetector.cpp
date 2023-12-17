#include "../../include/fileServices/FileChangeDetector.h"
#include <stack>
#include <unordered_set>


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
    std::stack<std::pair<std::shared_ptr<MerkleTree::Node>, std::shared_ptr<MerkleTree::Node>>> stack;

    stack.push({oldMerkleTree.getRoot(), newMerkleTree.getRoot()});

    while (!stack.empty()) {
        auto [oldNode, newNode] = stack.top();
        stack.pop();

        if (!oldNode && !newNode) {
            continue;
        }

        if (!oldNode || !newNode || oldNode->hash != newNode->hash) {
            FileChange::Type changeType = !oldNode ? FileChange::Type::Added :
                                          !newNode ? FileChange::Type::Deleted : FileChange::Type::Modified;

            changes.push_back(FileChange(newNode ? newNode->path : oldNode->path, changeType));
            continue;
        }

        std::unordered_set<std::string> oldPaths, newPaths;
        for (const auto& child : oldNode->children) oldPaths.insert(child->path);
        for (const auto& child : newNode->children) newPaths.insert(child->path);

        for (const auto& child : oldNode->children) {
            if (newPaths.count(child->path) == 0) {
                changes.push_back(FileChange(child->path, FileChange::Type::Deleted));
            } else {
                stack.push({child, newNode->findChildByPath(child->path)});
            }
        }

        for (const auto& child : newNode->children) {
            if (oldPaths.count(child->path) == 0) {
                changes.push_back(FileChange(child->path, FileChange::Type::Added));
            }
        }
    }

    return changes;
}

// Implement other helper methods as needed.
