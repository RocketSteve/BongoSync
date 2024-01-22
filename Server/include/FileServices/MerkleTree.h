#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <string>
#include <vector>
#include <memory>
#include <filesystem> // Include filesystem for directory handling
#include "HashCalculator.h"
#include <nlohmann/json.hpp>

class MerkleTree {
public:
    struct Node {
        std::string hash;
        std::string path;  // Store file or directory path
        bool isFile;       // Flag to distinguish files from directories
        std::vector<std::shared_ptr<Node>> children; // Children nodes

        // Constructor for directory nodes
        Node(const std::string& path, bool isFile = false)
                : path(path), isFile(isFile), hash("") {}

        // Constructor for file nodes
        Node(const std::string& path, const std::string& hash)
                : path(path), isFile(true), hash(hash) {}

        std::shared_ptr<Node> findChildByPath(const std::string& path) const;
    };

    MerkleTree();
    ~MerkleTree();
    void buildTree(const std::string& directoryPath);
    std::string getTreeHash() const;
    std::shared_ptr<MerkleTree::Node> getRoot() const;
    void printTree();

    int getNumberOfNodes() const;
    std::string getNodeHash(const std::string& path) const;
    bool isChildOf(const std::string& parentPath, const std::string& childPath) const;
    bool contains(const std::string& path) const;
    bool areAllLeavesFiles() const;
    bool areAllInternalNodesDirectories() const;
    int getTreeDepth() const;


    static MerkleTree loadFromFile(const std::string& filePath);

    void saveToFile(const std::string& filePath) const;



private:
    void deserializeTreeFromJson(std::shared_ptr<Node>& node, const nlohmann::json& jsonNode);
    void serializeTreeToJson(const std::shared_ptr<Node>& node, nlohmann::json& jsonNode) const;

    std::shared_ptr<Node> root;
    void buildTreeRecursive(std::shared_ptr<Node>& node, const std::filesystem::path& path);
    std::string combineHashes(const std::vector<std::shared_ptr<Node>>& children) const;
    void printTreeRecursive(std::shared_ptr<Node> node, int depth = 0, const std::string& childPosition = "") const;

    std::shared_ptr<Node> findNodeByPath(std::shared_ptr<Node> node, const std::string& path) const;
    int getTreeDepthRecursive(std::shared_ptr<Node> node) const;
    bool areAllLeavesFilesRecursive(std::shared_ptr<Node> node) const;
    bool areAllInternalNodesDirectoriesRecursive(std::shared_ptr<Node> node) const;
    int getNumberOfNodesRecursive(std::shared_ptr<Node> node) const;
};

#endif // MERKLETREE_H
