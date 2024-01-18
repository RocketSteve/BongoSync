#include "../../include/Communciation/MessageBuilder.h"

MessageBuilder& MessageBuilder::setUserExists(bool exists) {
    this->userExists = exists;
    return *this;
}

MessageBuilder& MessageBuilder::setUserAdded(bool added) {
    this->userAdded = added;
    return *this;
}

MessageBuilder& MessageBuilder::setAhead(std::string ahead) {
    this->ahead = ahead;
    return *this;
}

MessageBuilder& MessageBuilder::setTreeHash(std::string treeHash) {
    this->treeHash = treeHash;
    return *this;
}


std::string MessageBuilder::buildUserExistsMessage() const {
    nlohmann::json msg;
    msg["response"] = "user_exists";
    msg["data"]["exists"] = userExists;
    return msg.dump();
}

std::string MessageBuilder::buildUserAddedMessage() const {
    nlohmann::json msg;
    msg["response"] = "user_added";
    msg["data"]["added"] = userAdded;
    return msg.dump();
}

std::string MessageBuilder::buildPasswordCorrectMessage() const {
    nlohmann::json msg;
    msg["response"] = "password_correct";
    return msg.dump();
}

std::string MessageBuilder::buildPasswordIncorrectMessage() const {
    nlohmann::json msg;
    msg["response"] = "password_incorrect";
    return msg.dump();
}

std::string MessageBuilder::buildLatestVersionMessage() const {
    nlohmann::json msg;
    msg["response"] = "latest";
    return msg.dump();
}

std::string MessageBuilder::buildNotLatestVersionMessage() const {
    nlohmann::json msg;
    msg["response"] = "not_latest";
    msg["data"]["ahead"] = ahead;
    return msg.dump();
}

