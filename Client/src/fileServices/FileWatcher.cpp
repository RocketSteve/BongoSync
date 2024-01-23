#include "../../include/fileServices/FileWatcher.h"
#include "../../include/commandHandlers/HandleSync.h" // Include the HandleSync header

FileWatcher::FileWatcher() : loop(uv_default_loop()), isRunning(false) {}

FileWatcher::~FileWatcher() {
    stop();
}

void FileWatcher::initialize(const std::string& directoryPath) {
    directoryToWatch = directoryPath;
}

FileWatcher& FileWatcher::getInstance() {
    static FileWatcher instance;
    return instance;
}

void FileWatcher::start() {
    if (isRunning) {
        return; // Already running
    }

    uv_fs_event_init(loop, &fsEvent);
    fsEvent.data = this;
    uv_fs_event_start(&fsEvent, FileWatcher::onFileChange, directoryToWatch.c_str(), 0);
    uv_run(loop, UV_RUN_DEFAULT);

    isRunning = true;
}

void FileWatcher::stop() {
    if (!isRunning) {
        return; // Not running
    }

    uv_fs_event_stop(&fsEvent);
    isRunning = false;
}

void FileWatcher::onFileChange(uv_fs_event_t* handle, const char* filename, int events, int status) {
    // Create an instance of HandleSync and call the initiateSync method

    std::string directoryPath = Utility::getDefaultDirectory();
    HandleSync syncHandler(directoryPath);
    syncHandler.initiateSync();
}