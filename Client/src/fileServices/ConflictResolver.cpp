#include "../../include/fileServices/ConflictResolver.h"
#include <filesystem>

ConflictResolver::ConflictResolver(ServerCommunicator& communicator)
        : serverCommunicator(communicator) {}

void ConflictResolver::resolveConflict(const std::string& filePath) {
    renameFileForConflict(filePath);
    requestServerVersion(filePath);
}

void ConflictResolver::renameFileForConflict(const std::string& filePath) {
    std::filesystem::path p(filePath);
    std::filesystem::path newFilePath = p.parent_path() / (p.stem().string() + "_conflict" + p.extension().string());

    if (std::filesystem::exists(p)) {
        std::filesystem::rename(p, newFilePath);
    }
}

void ConflictResolver::requestServerVersion(const std::string& filePath) {
    // Use MessageCreator and MessageBuilder to build a file request message
    std::string request = MessageCreator::create()
            .setFilePath(filePath)
            .buildFileRequestMessage();

    // Send request to server
    serverCommunicator.sendMessage(request);

    serverCommunicator.receiveFile(filePath);
}
