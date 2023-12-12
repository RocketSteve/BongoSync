#ifndef CLIENT_HANDLESHARE_H
#define CLIENT_HANDLESHARE_H

#include <string>

class HandleShare {
public:
    void initiateShare(const std::string& email);
private:
    bool checkWithServer(const std::string& email);
};

#endif //CLIENT_HANDLESHARE_H
