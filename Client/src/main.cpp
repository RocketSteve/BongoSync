#include <iostream>
#include <string>
#include <map>
#include "../include/commandHandlers/HandleRegister.h"
#include "../include/commandHandlers/HandleStart.h"
#include "../include/commandHandlers/HandleShare.h"
#include "../include/commandHandlers/HandleUnshare.h"
#include "../include/commandHandlers/HandleBreak.h"

// Forward declarations of functions for each command
void handleRegister();
void handleStart();
void handleShare(const std::string& email);
void handleUnshare(const std::string& email);
void handleBreak();

int main(int argc, char** argv) {
    // Check if the first argument is 'bongo'
    if (argc < 3 || std::string(argv[1]) != "bongo") {
        std::cerr << "Usage: bongo <command> [options]\n";
        return 1;
    }

    std::string command = argv[2];
    std::map<std::string, std::function<void()>> commands = {
            {"register", handleRegister},
            {"start", handleStart},
            {"break", handleBreak}
    };

    if (commands.find(command) != commands.end()) {
        commands[command]();
    } else if (command == "share" || command == "unshare") {
        if (argc < 4) {
            std::cerr << "Usage: bongo " << command << " <email>\n";
            return 1;
        }
        std::string email = argv[3];
        if (command == "share") {
            handleShare(email);
        } else {
            handleUnshare(email);
        }
    } else {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}


void handleRegister() {
    std::cout << "Handling register command...\n";
    HandleRegister regHandler;
    regHandler.initiateRegistration();
}

void handleStart() {
    std::cout << "Handling start command...\n";
    HandleStart startHandler;
    startHandler.initiateStart();
}

void handleShare(const std::string& email) {
    std::cout << "Handling share command for email: " << email << "\n";
    HandleShare shareHandler;
    shareHandler.initiateShare(email);
}

void handleUnshare(const std::string& email) {
    std::cout << "Handling unshare command for email: " << email << "\n";
    HandleUnshare unshareHandler;
    unshareHandler.initiateUnshare(email);
}

void handleBreak() {
    std::cout << "Handling break command...\n";
    HandleBreak breakHandler;
    breakHandler.initiateBreak();
}
