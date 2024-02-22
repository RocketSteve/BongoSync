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
    std::string email;
    std::string password;
    std::string hostname;

    //ask for ip change
    std::string ipChange;
    std::cout << "Do you want to change the server IP? (yes/no): ";
    std::cin >> ipChange;
    if(ipChange == "yes")
    {
        std::cout << "Enter new server IP: ";
        std::cin >> serverIP;
    }

    //input email, password and hostname
    // std::cout << "Enter email: ";
    // std::cin >> email;
    // std::cout << "Enter password: ";
    // std::cin >> password;
    std::cout << "Enter hostname: ";
    std::cin >> hostname;

    // Connect to the server
    auto& communicator = ServerCommunicator::getInstance();
    if (!communicator.connectToServer(serverIP, serverPort)) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return 1;
    }

    //ask for registration or login
    std::string choice;
    std::cout << "Do you want to register or login? (register/login): ";
    std::cin >> choice;
    std::cout << "Enter email: ";
    std::cin >> email;
    std::cout << "Enter password: ";
    std::cin >> password;

    // if
    if(choice == "register")
    {
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
    }
    else if(choice == "login")
    {
        //Create login message
        std::string loginMessage = MessageCreator::create()
                .setEmail(email)
                .setPassword(password)
                .buildLoginMessage();

        // Send the login message
        if (!communicator.sendMessage(loginMessage)) {
            std::cerr << "Failed to send login message." << std::endl;
            communicator.closeConnection();
            return 1;
        }

        // Wait for server response
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Adjust duration as needed
        std::string serverResponse = communicator.receiveMessage();

        if (!serverResponse.empty()) {
            nlohmann::json responseJson = nlohmann::json::parse(serverResponse);

            if (responseJson["response"] == "password_correct") {
                std::cout << "Login successful." << std::endl;

                // Here, proceed with file sending or other actions as required
                // For example:
                // std::string filePath = "/path/to/file";
                // communicator.sendFile(filePath);
            } else if (responseJson["response"] == "password_incorrect") {
                std::cout << "Login failed." << std::endl;
            } else {
                std::cout << "Unexpected server response: " << serverResponse << std::endl;
            }
        } else {
            std::cerr << "No response received from the server." << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid choice" << std::endl;
    }

    // Close the connection
    communicator.closeConnection();

    return 0;
}
