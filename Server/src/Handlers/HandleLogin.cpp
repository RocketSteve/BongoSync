#include "../include/Handlers/HandleLogin.h"
#include "Database/UserManager.h"

bool HandleLogin::checkCredentials(const std::string& email, const std::string& passwordHash) {
    UserManager userManager;
    auto user = userManager.getUser(email);

    if (!user.empty()) {

        auto lastSpacePos = user.find_last_of(' ');

// Find the position of the space before the last space
        auto secondLastSpacePos = user.find_last_of(' ', lastSpacePos - 1);

// Find the position of the space before the second last space
        auto thirdLastSpacePos = user.find_last_of(' ', secondLastSpacePos - 1);

// Extract the hash value
        std::string storedHash = user.substr(thirdLastSpacePos + 1, secondLastSpacePos - thirdLastSpacePos - 1);


        std::cout << "Stored hash: " << storedHash << std::endl;
        std::cout << "Password hash: " << passwordHash << std::endl;
        if (storedHash == passwordHash) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}
