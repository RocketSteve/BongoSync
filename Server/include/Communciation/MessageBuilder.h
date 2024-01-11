#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <string>
#include <nlohmann/json.hpp>

class MessageBuilder {
public:
    MessageBuilder& setUserExists(bool exists);
    MessageBuilder& setUserAdded(bool added);

    std::string buildUserExistsMessage() const;
    std::string buildUserAddedMessage() const;
    std::string buildPasswordCorrectMessage() const;
    std::string buildPasswordIncorrectMessage() const;

private:
    bool userExists;
    bool userAdded;
};

#endif // MESSAGEBUILDER_H
