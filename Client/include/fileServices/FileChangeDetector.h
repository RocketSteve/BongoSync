#ifndef FILECHANGEDETECTOR_H
#define FILECHANGEDETECTOR_H

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
    FileChangeDetector(const std::string& directoryPath);
    std::vector<FileChange> detectChanges();

private:
    std::string directoryPath;
    MerkleTree oldMerkleTree;
    MerkleTree newMerkleTree;

    void buildInitialTree();
    void rebuildTree();
    std::vector<FileChange> compareTrees();
};

#endif // FILECHANGEDETECTOR_H
