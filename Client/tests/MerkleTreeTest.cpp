#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/fileServices/MerkleTree.h"


class MerkleTreeTest : public ::testing::Test {
protected:
    MerkleTree* tree;

    void SetUp() override {
        tree = new MerkleTree();
    }

    void TearDown() override {
        delete tree;
    }
};

TEST_F(MerkleTreeTest, Constructor) {
    // Test if the tree is correctly initialized
    ASSERT_TRUE(tree->getRoot() == nullptr);
}

TEST_F(MerkleTreeTest, BuildTree) {
    std::string testDirectory = "../tests/test_data";

    std::cout << std::filesystem::current_path() << std::endl;

    tree->buildTree(testDirectory);
    tree->printTree();

    ASSERT_NE(tree->getRoot(), nullptr);
    ASSERT_EQ(tree->getNumberOfNodes(), 6);

    ASSERT_EQ(tree->getNodeHash("../tests/test_data/test1.txt"), "af1349b9f5f9a1a6a0404dea36dcc9499bcb25c9adc112b7cc9a93cae41f3262");
    ASSERT_TRUE(tree->isChildOf("../tests/test_data", "../tests/test_data/sub_dir"));
    ASSERT_TRUE(tree->contains("../tests/test_data/test1.txt"));
    ASSERT_TRUE(tree->areAllLeavesFiles());
    ASSERT_TRUE(tree->areAllInternalNodesDirectories());
    ASSERT_EQ(tree->getTreeDepth(), 3);
}

TEST_F(MerkleTreeTest, EmptyDirectory) {
    std::string emptyDirectory = "../tests/empty_directory";
    tree->buildTree(emptyDirectory);
    ASSERT_NE(tree->getRoot(), nullptr);
    ASSERT_EQ(tree->getNumberOfNodes(), 1); // Only the root directory node
    ASSERT_TRUE(tree->areAllLeavesFiles());
    ASSERT_TRUE(tree->areAllInternalNodesDirectories());
    ASSERT_EQ(tree->getTreeDepth(), 1);

    tree->printTree();
}

TEST_F(MerkleTreeTest, NonExistentDirectory) {
    std::string nonExistentDirectory = "../tests/non_existent_directory/";
    tree->buildTree(nonExistentDirectory);
    ASSERT_EQ(tree->getRoot(), nullptr);
}

TEST_F(MerkleTreeTest, SingleFile) {
    std::string singleFileDirectory = "../tests/test_data/sub_dir";
    tree->buildTree(singleFileDirectory);
    ASSERT_NE(tree->getRoot(), nullptr);
    ASSERT_EQ(tree->getNumberOfNodes(), 2); // Root and the file node
    ASSERT_TRUE(tree->contains("../tests/test_data/sub_dir/test4.txt"));
    ASSERT_EQ(tree->getTreeDepth(), 2);
}


TEST_F(MerkleTreeTest, HashConsistency) {
    std::string directory = "../tests/test_data";
    tree->buildTree(directory);
    std::string firstHash = tree->getTreeHash();
    tree->buildTree(directory);
    std::string secondHash = tree->getTreeHash();
    ASSERT_EQ(firstHash, secondHash);
}
