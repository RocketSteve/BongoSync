#include "../include/HandleRegister.h"
#include <iostream>

void HandleRegister::initiateRegistration() {
    std::string email = promptForEmail();
    std::string password;
    std::string hostname = getHostname();

    do {
        password = promptForPassword();
    } while (!confirmPassword(password));

    if (checkWithServer(email, password, hostname)) {
        std::cout << "User already exists, please try again.\n";
        initiateRegistration();
    } else {
        createConfigFile(email, password, hostname);
    }
}

std::string HandleRegister::promptForEmail() {
    std::string email;
    std::cout << "Enter your email: ";
    std::getline(std::cin, email);
    return email;
}

std::string HandleRegister::promptForPassword() {
    std::string password;
    std::cout << "Enter your password: ";
    // You might want to implement a more secure way to read the password
    std::getline(std::cin, password);
    return password;
}

bool HandleRegister::confirmPassword(const std::string& password) {
    std::string confirmPassword;
    std::cout << "Confirm your password: ";
    std::getline(std::cin, confirmPassword);

    if (password != confirmPassword) {
        std::cout << "Passwords do not match, please try again.\n";
        return false;
    }
    return true;
}

std::string HandleRegister::getHostname() {
    // Implementation of getting hostname
    std::string hostname = "localhost";

    return hostname;
}

bool HandleRegister::checkWithServer(const std::string& email, const std::string& password, const std::string& hostname) {
    // Implementation of checking with server
    bool response;

    std::cout << "Checking with server ...\n" << email << "\n" << password << "\n" << hostname << "\n";


    if (response) {
        return true;
    } else {
        return false;
    }
}

void HandleRegister::createConfigFile(const std::string& email, const std::string& password, const std::string& hostname) {
    // Implementation of creating config file

    std::cout  << "Creating config file ...\n" << email << "\n" << password << "\n" << hostname << "\n";
}
