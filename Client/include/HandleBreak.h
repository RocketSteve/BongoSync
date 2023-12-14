#ifndef CLIENT_HANDLEBREAK_H
#define CLIENT_HANDLEBREAK_H

// TODO add file watcher stop

#include "../include/FileWatcher.h"
#include "../include/ServerCommunicator.h"

class HandleBreak {
public:
    void initiateBreak();
private:
    void stopFileWatcher();
    void closeConnection();
};

#endif //CLIENT_HANDLEBREAK_H
