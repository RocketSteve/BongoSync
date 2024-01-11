#include "../include/Communciation/MessageBuilder.h"

MessageBuilder& MessageBuilder::setUserExists(bool exists) {
    this->userExists = exists;
    return *this;
}

MessageBuilder& MessageBuilder::setUserAdded(bool added) {
    this->userAdded = added;
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

