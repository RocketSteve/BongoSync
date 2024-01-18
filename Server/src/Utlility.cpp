#include "../include/Utility.h"

 std::string Utility::getIPAddress() {
    std::string command;
#ifdef __APPLE__
    command = "ipconfig getifaddr en0"; // For MacOS
#else
    command = "hostname -I | cut -d' ' -f1"; // For Linux
#endif

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
