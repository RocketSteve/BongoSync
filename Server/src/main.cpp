#include "../include/ClientCommunicator.h"
#include "../include/Database/Database.h"
#include "../include/Utility.h"
#include <iostream>

int main() {

    // Initialize the database
    Database db;
    db.initialize();

    // Choose an appropriate port number
    int port = 12345;

    // Initialize and start the communicator
    ClientCommunicator communicator(port);
    communicator.start();

    // Run indefinitely (or until some condition is met)
    while (true) {
        // Here, you can add any additional logic or checks as necessary
        // For example, checking for server shutdown commands, etc.
    }

    // Stop the communicator when done
    communicator.stop();

    return 0;
}
