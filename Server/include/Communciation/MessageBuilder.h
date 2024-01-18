#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <string>
#include <nlohmann/json.hpp>

class MessageBuilder {
public:
    MessageBuilder& setUserExists(bool exists);
    MessageBuilder& setUserAdded(bool added);
    MessageBuilder& setAhead(std::string ahead);
    MessageBuilder& setTreeHash(std::string treeHash);

    std::string buildUserExistsMessage() const;
    std::string buildUserAddedMessage() const;
    std::string buildPasswordCorrectMessage() const;
    std::string buildPasswordIncorrectMessage() const;
    std::string buildLatestVersionMessage() const;
    std::string buildNotLatestVersionMessage() const;

private:
    bool userExists;
    bool userAdded;
    std::string ahead;
    std::string treeHash;
};

#endif // MESSAGEBUILDER_H
