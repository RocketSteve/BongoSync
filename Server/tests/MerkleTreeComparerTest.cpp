#include "../include/FileServices/MerkleTreeComparer.h"
#include "../include/FileServices/MerkleTree.h"
#include <gtest/gtest.h>
#include <memory>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
class MerkleTreeComparerTest : public ::testing::Test {
protected:
    fs::path baseDir;
    fs::path currentDir;
    MerkleTree baseTree;
    MerkleTree currentTree;

    void SetUp() override {
        // Create a base directory structure for tests
        baseDir = fs::temp_directory_path() / "baseTree";
        currentDir = fs::temp_directory_path() / "currentTree";
        createTestDirectory(baseDir);
        createTestDirectory(currentDir);

        // Build trees from these directories
        baseTree.buildTree(baseDir.string());
    }

    void TearDown() override {
        // Clean up created directories
        fs::remove_all(baseDir);
        fs::remove_all(currentDir);
    }

    void createTestDirectory(const fs::path& dir) {
        fs::create_directories(dir);
        std::ofstream(dir / "file1.txt") << "Content of file1";
        fs::create_directories(dir / "subdir");
        std::ofstream(dir / "subdir/file3.txt") << "Content of file3";
    }

    void modifyForRemovedTest() {
        fs::remove((currentDir / "file1.txt").string());
        currentTree.buildTree(currentDir.string());
    }

    void modifyForMultipleRemovedTest() {
        fs::remove((currentDir / "file1.txt").string());
        fs::remove((currentDir / "subdir/file3.txt").string());
        currentTree.buildTree(currentDir.string());
    }

    void modifyForAddedTest() {
        std::ofstream newFile((currentDir / "newfile.txt").string());
        newFile << "New file content";
        newFile.close();
        currentTree.buildTree(currentDir.string());
    }

    void modifyForMultipleAdditionsAndDeletionsTest() {
        // Remove some existing files
        fs::remove((currentDir / "file1.txt").string());
        fs::remove((currentDir / "subdir/file3.txt").string());

        // Add some new files
        std::ofstream newFile1((currentDir / "newfile1.txt").string());
        newFile1 << "New file 1 content";
        newFile1.close();

        std::ofstream newFile2((currentDir / "newfile2.txt").string());
        newFile2 << "New file 2 content";
        newFile2.close();

        // Rebuild the current tree
        currentTree.buildTree(currentDir.string());
    }
};

TEST_F(MerkleTreeComparerTest, TestNodeAdded) {
    modifyForAddedTest();
    MerkleTree currentTree;
    currentTree.buildTree(currentDir.string());
    MerkleTreeComparer comparer(baseTree, currentTree);
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();
    ASSERT_EQ(addedNodes.size(), 1);
    std::cout << addedNodes[0] << std::endl;
    EXPECT_EQ(addedNodes[0], "newfile.txt");
}


TEST_F(MerkleTreeComparerTest, TestEmptyTrees) {
    fs::remove_all(baseDir);
    fs::remove_all(currentDir);
    baseTree.buildTree(baseDir.string());
    MerkleTree currentTree;
    currentTree.buildTree(currentDir.string());
    MerkleTreeComparer comparer(baseTree, currentTree);
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();
    auto removedNodes = comparer.getRemovedNodes();
    auto modifiedNodes = comparer.getModifiedNodes();

    EXPECT_TRUE(addedNodes.empty());
    EXPECT_TRUE(removedNodes.empty());
    EXPECT_TRUE(modifiedNodes.empty());
}

TEST_F(MerkleTreeComparerTest, TestIdenticalTrees) {
    MerkleTree currentTree;
    currentTree.buildTree(currentDir.string());
    MerkleTreeComparer comparer(baseTree, currentTree);
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();
    auto removedNodes = comparer.getRemovedNodes();
    auto modifiedNodes = comparer.getModifiedNodes();

    EXPECT_TRUE(addedNodes.empty());
    EXPECT_TRUE(removedNodes.empty());
    EXPECT_TRUE(modifiedNodes.empty());
}

TEST_F(MerkleTreeComparerTest, TestMultipleNodesRemoved) {
    modifyForMultipleRemovedTest();
    MerkleTree currentTree;
    currentTree.buildTree(currentDir.string());
    MerkleTreeComparer comparer(baseTree, currentTree);
    comparer.compare();
    auto removedNodes = comparer.getRemovedNodes();
    ASSERT_EQ(removedNodes.size(), 2);
    EXPECT_EQ(removedNodes[1], "file1.txt");
    EXPECT_EQ(removedNodes[0], "subdir/file3.txt");
}

TEST_F(MerkleTreeComparerTest, TestMultipleNodesAddedAndRemoved) {
    modifyForMultipleAdditionsAndDeletionsTest();
    MerkleTree currentTree;
    currentTree.buildTree(currentDir.string());
    MerkleTreeComparer comparer(baseTree, currentTree);
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();
    auto removedNodes = comparer.getRemovedNodes();

    // Check that the correct nodes have been added
    ASSERT_EQ(addedNodes.size(), 2);
    EXPECT_EQ(addedNodes[0], "newfile1.txt");
    EXPECT_EQ(addedNodes[1], "newfile2.txt");

    // Check that the correct nodes have been removed
    ASSERT_EQ(removedNodes.size(), 2);
    EXPECT_EQ(removedNodes[1], "file1.txt");
    EXPECT_EQ(removedNodes[0], "subdir/file3.txt");
}