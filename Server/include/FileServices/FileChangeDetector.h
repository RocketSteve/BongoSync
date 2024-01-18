#ifndef FILECHANGEDETECTOR_H
#define FILECHANGEDETECTOR_H
//Server is supposed to receive serialized tree hash and deserialize it to MerkleTree object.
//Then it should compare it with its own tree.
//If there are any differences:
//-server vesrion newer: send files to client
//-client version newer: request file transfer from client
//There will be a semaphore on server side to prevent multiple operations on database at the same time as well
//as to prevent multiple file transfers at the same time.


#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include "MerkleTree.h"

class FileChange {
public:
    enum class Type {
        Added,
        Deleted,
        Modified
    };

    FileChange(std::string filePath, Type changeType);

    std::string getFilePath() const;
    Type getChangeType() const;

private:
    std::string filePath;
    Type changeType;
};

class FileChangeDetector {
public:
    // Constructor for initializing with a deserialized tree
    explicit FileChangeDetector(const MerkleTree& deserializedTree);

    // Method to detect file changes between the old and new Merkle trees
    std::vector<FileChange> detectChanges(bool useAsNewTree);

    // Method to compare the old and new Merkle trees and identify changes
    std::vector<FileChange> compareTrees(bool compareForward);

private:
    MerkleTree oldMerkleTree;
    MerkleTree newMerkleTree;
    std::string directoryPath;

    // Method to build the new Merkle tree from the /files directory
    void buildNewTree();

    // Method to get the relative path from the /files directory
    std::string getRelativePath(const std::string& filePath);
};

#endif // FILECHANGEDETECTOR_H

