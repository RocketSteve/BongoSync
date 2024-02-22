#include "../include/ClientCommunicator.h"
#include "../include/Database/Database.h"
#include "../include/Utility.h"
#include <iostream>

int main() {

    Database db;
    db.initialize();


    int port = 12345;


    ClientCommunicator communicator(port);
    communicator.start();

    while (true) {

    }


    communicator.stop();

    return 0;
}
