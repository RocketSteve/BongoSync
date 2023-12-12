#ifndef CLIENT_HANDLESTART_H
#define CLIENT_HANDLESTART_H

#include <string>

class HandleStart {
public:
    void initiateStart();
private:
    bool configExists();
    std::string promptForPassword();
    bool validateWithServer(const std::string& password);

    void startFileServices();
    void getLatestVersion();
};

#endif //CLIENT_HANDLESTART_H
