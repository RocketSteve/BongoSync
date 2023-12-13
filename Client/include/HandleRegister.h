#ifndef HANDLEREGISTER_H
#define HANDLEREGISTER_H

#include <string>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>

class HandleRegister {
public:
    void initiateRegistration();
private:
    std::string promptForEmail();
    std::string promptForPassword();
    std::string getHostname();
    bool confirmPassword(const std::string& password);

    bool checkWithServer(const std::string& email, const std::string& password, const std::string& hostname);

    void createConfigFile(const std::string& email, const std::string& hostname);
};

#endif // HANDLEREGISTER_H
