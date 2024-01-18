#include "../../include/FileServices/MerkleTree.h"
#include <filesystem>
#include <iostream>

MerkleTree::MerkleTree() : root(nullptr) {}

MerkleTree::~MerkleTree() {}

void MerkleTree::buildTree(const std::string& directoryPath) {
    // Check if the directory exists
    if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
        root = nullptr;  // Set root to nullptr for non-existent directories
        return;
    }
    root = std::make_shared<Node>(directoryPath);
    buildTreeRecursive(root, std::filesystem::path(directoryPath));
}

std::string MerkleTree::getTreeHash() const {
    return root ? root->hash : "";
}

void MerkleTree::buildTreeRecursive(std::shared_ptr<Node> &node, const std::filesystem::path &path) {
    std::filesystem::path defaultDirectory = "files";
    if (std::filesystem::is_directory(path)) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            // Get the relative path from the default directory
            std::filesystem::path relativePath = std::filesystem::relative(entry.path(), defaultDirectory);

            auto child = std::make_shared<Node>(relativePath.string());
            node->children.push_back(child);

            if (std::filesystem::is_directory(entry.path())) {
                buildTreeRecursive(child, entry.path());
            } else {
                child->hash = HashCalculator::calculateHash(relativePath.string());
                child->isFile = true;
            }
        }
        node->hash = combineHashes(node->children);
    }
}

std::shared_ptr<MerkleTree::Node> MerkleTree::getRoot() const {
    return root;
}

std::string MerkleTree::combineHashes(const std::vector<std::shared_ptr<Node>>& children) const {
    std::string combinedHashes;
    for (const auto& child : children) {
        combinedHashes += child->hash;
    }
    return HashCalculator::calculateHash(combinedHashes);
}


std::string MerkleTree::serializeTree() const {
    nlohmann::json treeJson;

    serializeNode(treeJson, root);
    return treeJson.dump();
}

void MerkleTree::serializeNode(nlohmann::json& parentJson, const std::shared_ptr<Node>& node) const {
    if (!node) return;

    // Serialize the current node
    nlohmann::json nodeJson;
    nodeJson["path"] = node->path;
    nodeJson["hash"] = node->hash;
    // Serialize children
    for (const auto& child : node->children) {
        nlohmann::json childJson;
        serializeNode(childJson, child);
        nodeJson["children"].push_back(childJson);
    }
    parentJson.push_back(nodeJson);
}

void MerkleTree::saveTreeToFile(const std::string& serializedTree) {
    std::filesystem::path bongoDir = std::filesystem::path(getenv("HOME")) / ".bongo";
    std::filesystem::create_directories(bongoDir);
    std::filesystem::path filePath = bongoDir / "tree.json";

    std::ofstream fileStream(filePath);
    if (fileStream) {
        fileStream << serializedTree;
        std::cout << "MerkleTree saved to: " << filePath << std::endl;
    } else {
        std::cerr << "Failed to save MerkleTree to file: " << filePath << std::endl;
    }
}

std::string MerkleTree::loadTreeFromFile() {
    std::filesystem::path filePath = "tree/tree.json";

    std::ifstream fileStream(filePath);
    if (!fileStream) {
        std::cerr << "Failed to open MerkleTree file: " << filePath << std::endl;
        return "";
    }
    std::string serializedTree((std::istreambuf_iterator<char>(fileStream)),
                               std::istreambuf_iterator<char>());

    std::cout << "MerkleTree: " << serializedTree << std::endl;
    return serializedTree;
}

void MerkleTree::deserializeTree(const std::string& serializedTree) {
    nlohmann::json treeJson = nlohmann::json::parse(serializedTree);

    if (!treeJson.empty() && treeJson.is_array()) {
        deserializeNode(treeJson[0], root);
    }
}

void MerkleTree::deserializeNode(const nlohmann::json& nodeJson, std::shared_ptr<Node>& node) {
    if (!nodeJson.contains("path")) return;

    // Create a new node with the path
    node = std::make_shared<Node>(nodeJson["path"]);

    // Set hash and other properties
    node->hash = nodeJson.value("hash", "");

    // Deserialize children
    if (nodeJson.contains("children")) {
        for (const auto& childJson : nodeJson["children"]) {
            auto child = std::make_shared<Node>(""); // Temporary empty path, will be set in recursion
            node->children.push_back(child);
            deserializeNode(childJson, child);
        }
    }
}

std::shared_ptr<MerkleTree::Node> MerkleTree::Node::findChildByPath(const std::string& path) const {
    for (const auto& child : children) {
        if (child->path == path) {
            return child;
        }
    }
    return nullptr; // Return nullptr if no child with the given path is found
}


std::shared_ptr<MerkleTree::Node> MerkleTree::findNodeByPath(std::shared_ptr<Node> node, const std::string& path) const {
    if (!node || node->path == path) return node;
    for (const auto& child : node->children) {
        auto result = findNodeByPath(child, path);
        if (result) return result;
    }
    return nullptr;
}

int MerkleTree::getNumberOfNodes() const {
    return getNumberOfNodesRecursive(root);
}

int MerkleTree::getNumberOfNodesRecursive(std::shared_ptr<Node> node) const {
    if (!node) return 0;
    int count = 1; // Count this node
    for (const auto& child : node->children) {
        count += getNumberOfNodesRecursive(child);
    }
    return count;
}

std::string MerkleTree::getNodeHash(const std::string& path) const {
    auto node = findNodeByPath(root, path);
    return node ? node->hash : "";
}


bool MerkleTree::isChildOf(const std::string& parentPath, const std::string& childPath) const {
    auto parentNode = findNodeByPath(root, parentPath);
    if (!parentNode) return false;
    for (const auto& child : parentNode->children) {
        if (child->path == childPath) {
            return true;
        }
    }
    return false;
}

bool MerkleTree::contains(const std::string& path) const {
    return findNodeByPath(root, path) != nullptr;
}

bool MerkleTree::areAllLeavesFiles() const {
    return areAllLeavesFilesRecursive(root);
}

bool MerkleTree::areAllLeavesFilesRecursive(std::shared_ptr<Node> node) const {
    if (!node) return true;

    // If node is a directory and has no children, it's not considered a leaf.
    if (!node->isFile && node->children.empty()) return true;

    // If node is a file and has no children, check if it's a file.
    if (node->children.empty()) return node->isFile;

    // Recursively check all children.
    for (const auto& child : node->children) {
        if (!areAllLeavesFilesRecursive(child)) {
            return false;
        }
    }
    return true;
}

bool MerkleTree::areAllInternalNodesDirectories() const {
    return areAllInternalNodesDirectoriesRecursive(root);
}

bool MerkleTree::areAllInternalNodesDirectoriesRecursive(std::shared_ptr<Node> node) const {
    if (!node || node->children.empty()) return true;
    if (node->isFile) return false;
    for (const auto& child : node->children) {
        if (!areAllInternalNodesDirectoriesRecursive(child)) {
            return false;
        }
    }
    return true;
}

int MerkleTree::getTreeDepth() const {
    return getTreeDepthRecursive(root);
}

int MerkleTree::getTreeDepthRecursive(std::shared_ptr<Node> node) const {
    if (!node) return 0;
    int maxDepth = 0;
    for (const auto& child : node->children) {
        maxDepth = std::max(maxDepth, getTreeDepthRecursive(child));
    }
    return maxDepth + 1; // Add 1 for the current node's depth
}

void MerkleTree::printTreeRecursive(std::shared_ptr<Node> node, int depth, const std::string& childPosition) const {
    if (!node) return;

    std::string indentation = std::string(depth * 4, ' ');
    std::cout << indentation;

    if (!childPosition.empty()) {
        std::cout << childPosition << " Child - ";
    }

    std::cout << (node->isFile ? "Leaf" : "Internal") << " Node: ";
    std::cout << "Path: " << node->path << ", Hash: " << node->hash << std::endl;

    for (const auto& child : node->children) {
        printTreeRecursive(child, depth + 1, node->isFile ? "File" : "Dir");
    }
}

void MerkleTree::printTree() {
    printTreeRecursive(root, 0, "");
}
