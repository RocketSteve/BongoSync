#include "../../include/commandHandlers/HandleShare.h"
#include <iostream>

void HandleShare::initiateShare(const std::string& email) {
    if (checkWithServer(email)) {
        std::cout << "Share initiated ...\n" << email << std::endl;
    } else {
        std::cout << "Share not initiated ...\n" << email << std::endl;
    }
    std::cout << "Initiating share ...\n" << email << std::endl;
}

bool HandleShare::checkWithServer(const std::string& email) {
    std::cout << "Checking with server for email: " << email << "\n";

    MessageBuilder messageCreator;
    std::string message = MessageCreator::create()
            .setEmail(email)
            .buildShareMessage();

    auto& serverCommunicator = ServerCommunicator::getInstance();

    // Send the message
    if (!serverCommunicator.sendMessage(message)) {
        std::cerr << "Failed to send message to server.\n";
        return false;
    }

    // Receive the response
    std::string responseStr;
    if (!(responseStr = serverCommunicator.receiveMessage()).empty()) {
        std::cerr << "Failed to receive response from server.\n";
        return false;
    }

    // Handle the response
    try {
        auto responseJson = nlohmann::json::parse(responseStr);
        bool isSuccess = responseJson.value("success", false);
        std::string message = responseJson.value("message", "");

        std::cout << "Server response: " << message << "\n";
        return isSuccess;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
        return false;
    }
}
