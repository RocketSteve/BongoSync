#include "../../include/Communciation/Session.h"
#include <cstdlib>

Session::Session(const std::string& email) : email(email) {
    // Generate a unique session ID (this is a simplified example)
    sessionID = std::to_string(rand());
}

std::string Session::getSessionID() const {
    return sessionID;
}

std::string Session::getEmail() const {
    return email;
}
