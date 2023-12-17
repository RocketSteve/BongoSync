#include "../../include/fileServices/MerkleTree.h"
#include <filesystem>

MerkleTree::MerkleTree() : root(nullptr) {}

MerkleTree::~MerkleTree() {}

void MerkleTree::buildTree(const std::string& directoryPath) {
    std::vector<std::string> filePaths;
    getAllFilePaths(directoryPath, filePaths);
    buildTreeRecursive(root, filePaths, 0, filePaths.size() - 1);
}

std::string MerkleTree::getTreeHash() const {
    return root ? root->hash : "";
}

void MerkleTree::buildTreeRecursive(std::shared_ptr<Node>& node, const std::vector<std::string>& filePaths, int start, int end) {
    if (start > end) return;

    if (start == end) {
        node = std::make_shared<Node>(HashCalculator::calculateHash(filePaths[start]), filePaths[start]);
    } else {
        int mid = start + (end - start) / 2;
        node = std::make_shared<Node>("");
        buildTreeRecursive(node->left, filePaths, start, mid);
        buildTreeRecursive(node->right, filePaths, mid + 1, end);
        node->hash = combineHashes(node->left->hash, node->right->hash);
    }
}

std::shared_ptr<MerkleTree::Node> MerkleTree::getRoot() const {
    return root;
}


std::string MerkleTree::combineHashes(const std::string& leftHash, const std::string& rightHash) const {
    return HashCalculator::calculateHash(leftHash + rightHash);
}

void MerkleTree::getAllFilePaths(const std::string& directoryPath, std::vector<std::string>& filePaths) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
        if (!std::filesystem::is_directory(entry.path())) {
            filePaths.push_back(entry.path().string());
        }
    }
}


