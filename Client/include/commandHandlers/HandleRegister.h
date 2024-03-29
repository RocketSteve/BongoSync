#ifndef HANDLEREGISTER_H
#define HANDLEREGISTER_H

// TODO add:
// 1. responseprocessing maybe move to new class
// 2. message recieval

#include <string>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../Utility.h"
#include "../communication/ServerCommunicator.h"
#include "../communication/MessageBuilder.h"
#include "../communication/MessageCreator.h"
#include "../fileServices/MerkleTree.h"

class HandleRegister {
public:
    void initiateRegistration();
private:
    bool checkWithServer(const std::string& email, const std::string& password, const std::string& hostname, const std::string& serverIP, int serverPort);
    static void createConfigFile(const std::string& email, const std::string& hostname, const std::string& serverIP, int serverPort);
};

#endif // HANDLEREGISTER_H
