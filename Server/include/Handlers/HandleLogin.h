#ifndef HANDLELOGIN_H
#define HANDLELOGIN_H

#include <string>

class HandleLogin {
public:
    static bool checkCredentials(const std::string& email, const std::string& passwordHash);
};

#endif // HANDLELOGIN_H
