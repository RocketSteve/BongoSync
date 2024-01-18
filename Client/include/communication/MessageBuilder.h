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
    MessageBuilder& setFilePath(const std::string& filePath);
    MessageBuilder& setModifiedAt(const std::string& ModifiedAt);
    MessageBuilder& setFileSize(size_t fileSize);
    MessageBuilder& setRelativeFilePath(const std::string& relativeFilePath);
    MessageBuilder& setAhead(bool ahead);

    std::string buildFileRequestMessage() const;
    std::string buildRegistrationMessage() const;
    std::string buildLoginMessage() const;
    std::string buildAskIfLatestMessage() const;
    std::string buildDeleteMessage() const;
    std::string buildShareMessage() const;
    std::string buildFileMetadataMessage() const;

private:
    std::string email;
    std::string password;
    std::string hostname;
    std::string treeHash;
    std::string filePath;
    std::string modifiedAt;

    std::string relativeFilePath;
    bool ahead;
    size_t fileSize;
};

#endif // MESSAGEBUILDER_H
