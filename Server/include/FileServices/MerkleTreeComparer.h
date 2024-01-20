#ifndef MERKLETREECOMPARER_H
#define MERKLETREECOMPARER_H

#include "MerkleTree.h"
#include <vector>
#include <string>

class MerkleTreeComparer {
public:
    explicit MerkleTreeComparer(const MerkleTree& baseTree, const std::string& currentTreePath);
    void compare();
    std::vector<std::string> getAddedNodes() const;
    std::vector<std::string> getRemovedNodes() const;
    std::vector<std::string> getModifiedNodes() const;

private:
    MerkleTree baseTree;
    MerkleTree currentTree;
    std::vector<std::string> addedNodes;
    std::vector<std::string> removedNodes;
    std::vector<std::string> modifiedNodes;

    void compareTrees(std::shared_ptr<MerkleTree::Node> baseNode, std::shared_ptr<MerkleTree::Node> currentNode);
};

#endif // MERKLETREECOMPARER_H
