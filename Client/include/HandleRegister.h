#ifndef HANDLEREGISTER_H
#define HANDLEREGISTER_H

#include <string>

class HandleRegister {
public:
    void initiateRegistration();
private:
    std::string promptForEmail();
    std::string promptForPassword();
    std::string getHostname();
    bool confirmPassword(const std::string& password);

    bool checkWithServer(const std::string& email, const std::string& password, const std::string& hostname);

    void createConfigFile(const std::string& email, const std::string& password, const std::string& hostname);
};

#endif // HANDLEREGISTER_H
