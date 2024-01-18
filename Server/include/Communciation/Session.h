#ifndef SESSION_H
#define SESSION_H

#include <string>

class Session {
public:
    explicit Session(const std::string& email);
    std::string getSessionID() const;
    std::string getEmail() const;

private:
    std::string sessionID;
    std::string email;
};

#endif // SESSION_H
