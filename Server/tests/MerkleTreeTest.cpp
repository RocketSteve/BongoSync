#include "../include/FileServices/MerkleTree.h"
#include "../include/FileServices/MerkleTreeComparer.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>


class MerkleTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code, if needed
    }

    void TearDown() override {
        // Cleanup code, if needed
        std::filesystem::remove("test_tree.json");
    }

    // Utility function to create a simple test directory structure
    void createTestDirectory(const std::string& path) {
        std::filesystem::create_directories(path + "/dir1");
        std::filesystem::create_directories(path + "/dir2");
        std::ofstream(path + "/file1.txt").put('a');
        std::ofstream(path + "/dir1/file2.txt").put('b');
    }
};

TEST_F(MerkleTreeTest, SerializationTest) {
    createTestDirectory("test_dir");
    MerkleTree tree;
    tree.buildTree("test_dir");
    tree.saveToFile("test_tree.json");

    std::ifstream file("test_tree.json");
    ASSERT_TRUE(file.is_open());
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    ASSERT_FALSE(content.empty());
}

TEST_F(MerkleTreeTest, DeserializationTest) {
    createTestDirectory("test_dir");
    MerkleTree serializedTree;
    serializedTree.buildTree("test_dir");
    serializedTree.saveToFile("test_tree.json");
    // Assuming 'SerializationTest' has been run and 'test_tree.json' exists
    MerkleTree deserializedTree = MerkleTree::loadFromFile("test_tree.json");
    deserializedTree.printTree();
    ASSERT_NE(deserializedTree.getRoot(), nullptr);
    ASSERT_EQ(deserializedTree.getRoot()->children.size(), 3); // Replace with actual expected values
}

TEST_F(MerkleTreeTest, RoundTripTest) {
    createTestDirectory("test_dir");
    MerkleTree tree;
    tree.buildTree("test_dir");
    tree.saveToFile("test_tree.json");

    MerkleTree loadedTree = MerkleTree::loadFromFile("test_tree.json");
    ASSERT_EQ(tree.getTreeHash(), loadedTree.getTreeHash()); // Replace with more detailed checks if needed
}

TEST_F(MerkleTreeTest, ErrorHandlingTest) {
    MerkleTree tree = MerkleTree::loadFromFile("non_existent_file.json");
    ASSERT_EQ(tree.getRoot(), nullptr);
}