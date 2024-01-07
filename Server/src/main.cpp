#include "../include/ClientCommunicator.h"
#include <iostream>

int main() {
    // Choose an appropriate port number
    int port = 12345;

    // Initialize and start the communicator
    ClientCommunicator communicator(port);
    communicator.start();

    std::cout << "ClientCommunicator is running on port " << port << std::endl;

    // Run indefinitely (or until some condition is met)
    while (true) {
        // Add any additional logic or checks as necessary
    }

    // Stop the communicator when done
    communicator.stop();

    return 0;
}


