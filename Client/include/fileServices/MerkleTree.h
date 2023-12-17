#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <string>
#include <vector>
#include <memory>
#include "HashCalculator.h"

class MerkleTree {
public:
    MerkleTree();
    ~MerkleTree();
    void buildTree(const std::string& directoryPath);
    std::string getTreeHash() const;
    std::shared_ptr<MerkleTree::Node> getRoot() const; // Added getter for root

    struct Node {
        std::string hash;
        std::string filePath; // Store file path for leaf nodes
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        Node(const std::string& hash, const std::string& filePath = "") : hash(hash), filePath(filePath), left(nullptr), right(nullptr) {}
    };

private:

    std::shared_ptr<Node> root;
    void buildTreeRecursive(std::shared_ptr<Node>& node, const std::vector<std::string>& filePaths, int start, int end);
    std::string combineHashes(const std::string& leftHash, const std::string& rightHash) const;
    void getAllFilePaths(const std::string& directoryPath, std::vector<std::string>& filePaths);
};

#endif // MERKLETREE_H
