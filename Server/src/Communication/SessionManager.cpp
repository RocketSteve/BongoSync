#include "../../include/Communciation/SessionManager.h"

std::shared_ptr<Session> SessionManager::createSession(const std::string& email) {
    auto session = std::make_shared<Session>(email);
    sessions[session->getSessionID()] = session;
    return session;
}

std::shared_ptr<Session> SessionManager::getSession(const std::string& sessionID) {
    auto it = sessions.find(sessionID);
    if (it != sessions.end()) {
        return it->second;
    }
    return nullptr;
}

void SessionManager::endSession(const std::string& sessionID) {
    sessions.erase(sessionID);
}
