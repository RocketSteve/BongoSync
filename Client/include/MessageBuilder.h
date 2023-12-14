#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <string>
#include <nlohmann/json.hpp>

class MessageBuilder {
public:
    MessageBuilder& setEmail(const std::string& email);
    MessageBuilder& setPassword(const std::string& password);
    MessageBuilder& setHostname(const std::string& hostname);
    MessageBuilder& setTreeHash(const std::string& treeHash);

    std::string buildRegistrationMessage() const;
    std::string buildLoginMessage() const;
    std::string buildAskIfLatestMessage() const;

private:
    std::string email;
    std::string password;
    std::string hostname;
    std::string treeHash;
};

#endif // MESSAGEBUILDER_H
