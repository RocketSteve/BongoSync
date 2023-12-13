#ifndef CLIENT_MESSAGECREATOR_H
#define CLIENT_MESSAGECREATOR_H
#include <string>

class MessageCreator {
public:
    std::string createRegistratioMessage(const std::string& email, const std::string& password, const std::string& hostname);
    std::string askIfLatestMessage(const std::string& treeHash);
};

#endif //CLIENT_MESSAGECREATOR_H
