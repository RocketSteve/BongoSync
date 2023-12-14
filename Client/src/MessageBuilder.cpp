#include "../include/MessageBuilder.h"

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

// Build methods implementation
std::string MessageBuilder::buildRegistrationMessage() const {
    nlohmann::json msg;
    msg["action"] = "register";
    msg["data"]["email"] = email;
    msg["data"]["password"] = password;
    msg["data"]["hostname"] = hostname;
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
    return msg.dump();
}