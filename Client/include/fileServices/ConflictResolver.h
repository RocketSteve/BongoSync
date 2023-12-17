#ifndef CONFLICTRESOLVER_H
#define CONFLICTRESOLVER_H

#include <string>
#include "../communication/ServerCommunicator.h"
#include "../communication/MessageCreator.h"

class ConflictResolver {
public:
    explicit ConflictResolver(ServerCommunicator& communicator);
    void resolveConflict(const std::string& filePath);

private:
    ServerCommunicator& serverCommunicator;
    void renameFileForConflict(const std::string& filePath);
    void requestServerVersion(const std::string& filePath);
    void handleRemoteUpdate(const nlohmann::json& updateInfo);
};

#endif // CONFLICTRESOLVER_H
