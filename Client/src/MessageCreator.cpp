#include "../include/MessageCreator.h"
#include <nlohmann/json.hpp>

std::string MessageCreator::createRegistratioMessage(const std::string& email, const std::string& password, const std::string& hostname) {
    nlohmann::json j;
    j["email"] = email;
    j["password"] = password;
    j["hostname"] = hostname;
    return j.dump();
}