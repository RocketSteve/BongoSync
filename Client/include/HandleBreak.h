#ifndef CLIENT_HANDLEBREAK_H
#define CLIENT_HANDLEBREAK_H

class HandleBreak {
public:
    void initiateBreak();
private:
    void stopFileWatcher();
    void closeConnection();
};

#endif //CLIENT_HANDLEBREAK_H
