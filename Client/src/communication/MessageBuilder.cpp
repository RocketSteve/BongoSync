#include "../../include/communication/MessageBuilder.h"

MessageBuilder& MessageBuilder::setEmail(const std::string& email) {
    this->email = email;
    return *this;
}

MessageBuilder& MessageBuilder::setPassword(const std::string& password) {
    this->password = password;
    return *this;
}

MessageBuilder& MessageBuilder::setHostname(const std::string& hostname) {
    this->hostname = hostname;
    return *this;
}

MessageBuilder& MessageBuilder::setTreeHash(const std::string& treeHash) {
    this->treeHash = treeHash;
    return *this;
}

MessageBuilder& MessageBuilder::setModifiedAt(const std::string &ModifiedAt) {
    this->modifiedAt = ModifiedAt;
    return *this;
}

MessageBuilder& MessageBuilder::setRelativeFilePath(const std::string& filePath) {
    this->relativeFilePath = filePath;
    return *this;
}

MessageBuilder& MessageBuilder::setAhead(bool ahead) {
    this->ahead = ahead;
    return *this;
}

MessageBuilder& MessageBuilder::setFileSize(size_t fileSize) {
    this->fileSize = fileSize;
    return *this;
}

// Build methods implementation
std::string MessageBuilder::buildRegistrationMessage() const {
    nlohmann::json msg;
    msg["action"] = "register";
    msg["data"]["email"] = email;
    msg["data"]["password"] = password;
    msg["data"]["hostname"] = hostname;
    msg["data"]["tree_hash"] = treeHash;
    return msg.dump();
}

std::string MessageBuilder::buildLoginMessage() const {
    nlohmann::json msg;
    msg["action"] = "login";
    msg["data"]["email"] = email;
    msg["data"]["password"] = password;
    return msg.dump();
}

std::string MessageBuilder::buildAskIfLatestMessage() const {
    nlohmann::json msg;
    msg["action"] = "check_latest";
    msg["data"]["tree_hash"] = treeHash;
    msg["data"]["modified_at"] = modifiedAt;
    return msg.dump();
}

MessageBuilder& MessageBuilder::setFilePath(const std::string& filePath) {
    this->filePath = filePath;
    return *this;
}

std::string MessageBuilder::buildFileRequestMessage() const {
    nlohmann::json msg;
    msg["type"] = "file_request";
    msg["file_path"] = filePath;
    return msg.dump();
}

std::string MessageBuilder::buildDeleteMessage() const {
    nlohmann::json msg;
    msg["type"] = "delete";
    msg["file_path"] = filePath;
    return msg.dump();
}

std::string MessageBuilder::buildShareMessage() const {
    nlohmann::json msg;
    msg["type"] = "share";
    msg["data"]["email"] = email;
    return msg.dump();
}

// New method to build file metadata message
std::string MessageBuilder::buildFileMetadataMessage() const {
    nlohmann::json fileMetadata;
    fileMetadata["action"] = "tree";
    fileMetadata["data"]["file_name"] = relativeFilePath;
    fileMetadata["data"]["ahead"] = ahead;
    fileMetadata["data"]["file_size"] = fileSize;
    return fileMetadata.dump();
}