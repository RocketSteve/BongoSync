#ifndef CLIENT_HANDLESHARE_H
#define CLIENT_HANDLESHARE_H

#include <string>

class HandleUnshare {
public:
    void initiateUnshare(std::string& email);
private:
    bool checkWithServer(std::string& email);
};

#endif //CLIENT_HANDLESHARE_H
