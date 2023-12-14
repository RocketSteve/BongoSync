#ifndef HANDLEREGISTER_H
#define HANDLEREGISTER_H

#include <string>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../include/Utility.h"
#include "../include/ServerCommunicator.h"
#include "../include/MessageBuilder.h"
#include "../include/MessageCreator.h"

class HandleRegister {
public:
    void initiateRegistration();
private:
    bool checkWithServer(const std::string& email, const std::string& password, const std::string& hostname);
    void createConfigFile(const std::string& email, const std::string& hostname);
};

#endif // HANDLEREGISTER_H
