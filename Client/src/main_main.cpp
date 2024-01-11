#include "../include/communication/ServerCommunicator.h"
#include "../include/communication/MessageBuilder.h"
#include "../include/communication/MessageCreator.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

int main() {
    // Server details
    std::string serverIP = "172.16.113.175"; // Replace with your server IP
    int serverPort = 12345; // Port number should match the server's listening port

    // Registration details
    std::string email = "user@example.com";
    std::string password = "password123";
    std::string hostname = "userHost";

    // Connect to the server
    auto& communicator = ServerCommunicator::getInstance();
    if (!communicator.connectToServer(serverIP, serverPort)) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return 1;
    }

    // Create a registration message
    MessageBuilder messageCreator;
    std::string regMessage = MessageCreator::create()
            .setEmail(email)
            .setHostname(hostname)
            .setPassword(password)
            .buildRegistrationMessage();

    // Send the registration message
    if (!communicator.sendMessage(regMessage)) {
        std::cerr << "Failed to send registration message." << std::endl;
        communicator.closeConnection();
        return 1;
    }

    std::cout << "Registration message sent successfully." << std::endl;

    // Wait for server response
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Adjust duration as needed
    std::string serverResponse = communicator.receiveMessage();

    if (!serverResponse.empty()) {
        nlohmann::json responseJson = nlohmann::json::parse(serverResponse);

        if (responseJson["response"] == "user_exists") {
            std::cout << "User already exists." << std::endl;
        } else if (responseJson["response"] == "user_added") {
            std::cout << "Registration successful." << std::endl;

            // Here, proceed with file sending or other actions as required
            // For example:
            // std::string filePath = "/path/to/file";
            // communicator.sendFile(filePath);
        } else {
            std::cout << "Unexpected server response: " << serverResponse << std::endl;
        }
    } else {
        std::cerr << "No response received from the server." << std::endl;
    }

    // Close the connection
    communicator.closeConnection();

    return 0;
}
