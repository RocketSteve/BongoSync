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
};

TEST_F(MerkleTreeComparerTest, TestNodeAdded) {
    modifyForAddedTest();
    MerkleTreeComparer comparer(baseTree, currentDir.string());
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();
    ASSERT_EQ(addedNodes.size(), 1);
    EXPECT_EQ(addedNodes[0], "newfile.txt");
}

TEST_F(MerkleTreeComparerTest, TestNodeRemoved) {
    modifyForRemovedTest();

    MerkleTreeComparer comparer(baseTree, currentDir.string());
    comparer.compare();
    auto removedNodes = comparer.getRemovedNodes();

    ASSERT_EQ(removedNodes.size(), 1);
    EXPECT_EQ(removedNodes[0], "file1.txt");
}

TEST_F(MerkleTreeComparerTest, TestEmptyTrees) {
    fs::remove_all(baseDir);
    fs::remove_all(currentDir);
    baseTree.buildTree(baseDir.string());
    currentTree.buildTree(currentDir.string());

    MerkleTreeComparer comparer(baseTree, currentDir.string());
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();
    auto removedNodes = comparer.getRemovedNodes();
    auto modifiedNodes = comparer.getModifiedNodes();

    EXPECT_TRUE(addedNodes.empty());
    EXPECT_TRUE(removedNodes.empty());
    EXPECT_TRUE(modifiedNodes.empty());
}

TEST_F(MerkleTreeComparerTest, TestIdenticalTrees) {
    MerkleTreeComparer comparer(baseTree, currentDir.string());
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();
    auto removedNodes = comparer.getRemovedNodes();
    auto modifiedNodes = comparer.getModifiedNodes();

    EXPECT_TRUE(addedNodes.empty());
    EXPECT_TRUE(removedNodes.empty());
    EXPECT_TRUE(modifiedNodes.empty());
}

TEST_F(MerkleTreeComparerTest, TestMultipleNodesAdded) {
    std::ofstream newFile1((currentDir / "newfile1.txt").string());
    newFile1 << "New file content 1";
    newFile1.close();

    fs::create_directories(currentDir / "subdir/subsubdir");

    std::ofstream newFile2((currentDir / "newfile2.txt").string());
    newFile2 << "New file content 2";
    newFile2.close();

    std::ofstream newFile4((currentDir / "subdir/newfile4.txt").string());
    newFile4 << "New file content 1";
    newFile4.close();

    currentTree.buildTree(currentDir.string());

    MerkleTreeComparer comparer(baseTree, currentDir.string());
    comparer.compare();
    auto addedNodes = comparer.getAddedNodes();

    for (const auto& node : addedNodes) {
        std::cout << node << std::endl;
    }

    ASSERT_EQ(addedNodes.size(), 4);
}

TEST_F(MerkleTreeComparerTest, TestMultipleNodesRemoved) {
    fs::remove((currentDir / "file1.txt").string());
    fs::remove((currentDir / "subdir/file3.txt").string());
    currentTree.buildTree(currentDir.string());

    MerkleTreeComparer comparer(baseTree, currentDir.string());
    comparer.compare();
    auto removedNodes = comparer.getRemovedNodes();

    ASSERT_EQ(removedNodes.size(), 2);
}
