#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/fileServices/MerkleTree.h"


class MerkleTreeTest : public ::testing::Test {
protected:
    MerkleTree* tree;

    void SetUp() override {
        tree = new MerkleTree();
        // Additional setup can go here
    }

    void TearDown() override {
        delete tree;
    }
};

TEST_F(MerkleTreeTest, Constructor) {
    // Test if the tree is correctly initialized
    ASSERT_TRUE(tree->getRoot() == nullptr);
}

