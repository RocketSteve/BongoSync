#include <gtest/gtest.h>
#include "../include/communication/MessageCreator.h"
#include <nlohmann/json.hpp>

TEST(MessageBuilderTest, BuildRegistrationMessage) {
    auto message = MessageCreator::create()
            .setEmail("test@example.com")
            .setPassword("password123")
            .setHostname("TestHost")
            .buildRegistrationMessage();

    auto json = nlohmann::json::parse(message);
    EXPECT_EQ(json["action"], "register");
    EXPECT_EQ(json["data"]["email"], "test@example.com");
    EXPECT_EQ(json["data"]["password"], "password123");
    EXPECT_EQ(json["data"]["hostname"], "TestHost");
}

TEST(MessageBuilderTest, BuildLoginMessage) {
    auto message = MessageCreator::create()
            .setEmail("user@example.com")
            .setPassword("mypassword")
            .buildLoginMessage();

    auto json = nlohmann::json::parse(message);
    EXPECT_EQ(json["action"], "login");
    EXPECT_EQ(json["data"]["email"], "user@example.com");
    EXPECT_EQ(json["data"]["password"], "mypassword");
}

TEST(MessageBuilderTest, BuildAskIfLatestMessage) {
    auto message = MessageCreator::create()
            .setTreeHash("12345abcde")
            .buildAskIfLatestMessage();

    auto json = nlohmann::json::parse(message);
    EXPECT_EQ(json["action"], "check_latest");
    EXPECT_EQ(json["data"]["tree_hash"], "12345abcde");
}

TEST(MessageBuilderTest, BuildFileRequestMessage) {
    auto message = MessageCreator::create()
            .setFilePath("/path/to/file.txt")
            .buildFileRequestMessage();

    auto json = nlohmann::json::parse(message);
    EXPECT_EQ(json["type"], "file_request");
    EXPECT_EQ(json["file_path"], "/path/to/file.txt");
}

TEST(MessageBuilderTest, BuildDeleteMessage) {
    auto message = MessageCreator::create()
            .setFilePath("/path/to/delete.txt")
            .buildDeleteMessage();

    auto json = nlohmann::json::parse(message);
    EXPECT_EQ(json["type"], "delete");
    EXPECT_EQ(json["file_path"], "/path/to/delete.txt");
}

TEST(MessageBuilderTest, BuildShareMessage) {
    auto message = MessageCreator::create()
            .setEmail("share@example.com")
            .buildShareMessage();

    auto json = nlohmann::json::parse(message);
    EXPECT_EQ(json["type"], "share");
    EXPECT_EQ(json["data"]["email"], "share@example.com");
}


