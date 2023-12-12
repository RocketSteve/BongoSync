#ifndef CLIENT_HANDLESYNC_H
#define CLIENT_HANDLESYNC_H

class HandleSync {
public:
    void initiateSync();
private:
    void checkWithServer();
    void syncWithServer();
    void locConflict();
};

#endif //CLIENT_HANDLESYNC_H
