#ifndef CLIENT_HANDLEBREAK_H
#define CLIENT_HANDLEBREAK_H

#include "../fileServices/FileWatcher.h"
#include "../communication/ServerCommunicator.h"
#include "../Utility.h"

class HandleBreak {
public:
    void initiateBreak();

private:
    void stopFileWatcher();
    void closeConnection();
};

#endif //CLIENT_HANDLEBREAK_H
