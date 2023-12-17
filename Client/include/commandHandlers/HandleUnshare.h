#ifndef HANDLE_UNSHARE_H
#define HANDLE_UNSHARE_H

// TODO implement ushare

#include <string>
#include <iostream>
#include "../Utility.h"

class HandleUnshare {
public:
    void initiateUnshare(const std::string& email);
private:
    bool checkWithServer(const std::string& email);
};

#endif // HANDLE_UNSHARE_H
