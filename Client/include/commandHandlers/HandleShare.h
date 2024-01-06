#ifndef CLIENT_HANDLESHARE_H
#define CLIENT_HANDLESHARE_H

// TODO implement share

#include <string>
#include "../Utility.h"
#include "../communication/ServerCommunicator.h"
#include "../communication/MessageCreator.h"
#include "../communication/MessageBuilder.h"

class HandleShare {
public:
    void initiateShare(const std::string& email);
private:
    bool checkWithServer(const std::string& email);
};

#endif //CLIENT_HANDLESHARE_H
