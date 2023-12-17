#include "../../include/fileServices/FileWatcher.h"

FileWatcher::FileWatcher() : loop(uv_default_loop()), fileChangeDetector(nullptr), isRunning(false) {}


FileWatcher::~FileWatcher() {
    stop();
    delete fileChangeDetector; // Clean up
}

void FileWatcher::initialize(const std::string& directoryPath) {
    directoryToWatch = directoryPath;
    fileChangeDetector = new FileChangeDetector(directoryPath); // Initialize with the given path
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
    FileWatcher* watcher = static_cast<FileWatcher*>(handle->data);
    if (watcher && filename) {
        // Use FileChangeDetector to detect changes, using -> since fileChangeDetector is a pointer
        auto changes = watcher->fileChangeDetector->detectChanges();
        for (const auto& change : changes) {
            // Process the changes as needed
            // Example: std::cout << "Change detected in file: " << change.getFilePath() << std::endl;
        }
    }
}



