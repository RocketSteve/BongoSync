#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include "HashCalculator.h"

class MerkleNode {
public:
    std::string hash;
    std::vector<MerkleNode> children;

    MerkleNode(const std::string& hash) : hash(hash) {}
};

class CollisionDetector {
public:
    CollisionDetector(const std::string& rootDirectory);
    void buildTree();
    void handleFileChange(const std::string& filename, int events);

private:
    std::string rootDirectory;
    MerkleNode rootNode;

    MerkleNode buildTreeForDirectory(const std::string& directory);
    std::string calculateDirectoryHash(const MerkleNode& node);
};

#endif // COLLISIONDETECTOR_H
