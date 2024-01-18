#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "Session.h"
#include <memory>
#include <unordered_map>

class SessionManager {
public:
    std::shared_ptr<Session> createSession(const std::string& email);
    std::shared_ptr<Session> getSession(const std::string& sessionID);
    void endSession(const std::string& sessionID);

private:
    std::unordered_map<std::string, std::shared_ptr<Session>> sessions;
};

#endif // SESSIONMANAGER_H
