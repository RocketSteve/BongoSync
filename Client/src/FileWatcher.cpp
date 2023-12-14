#include "../include/FileWatcher.h"


FileWatcher::FileWatcher() : loop(uv_default_loop()), collisionDetector(nullptr), isRunning(false) {}

FileWatcher::~FileWatcher() {
    stop();
}

FileWatcher& FileWatcher::getInstance() {
    static FileWatcher instance;
    return instance;
}

void FileWatcher::start(const std::string& path, CollisionDetector& detector) {
    if (isRunning) {
        return; // Already running
    }

    directoryToWatch = path;
    collisionDetector = &detector;

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
    if (watcher && watcher->collisionDetector && filename) {
        // Notify the CollisionDetector class about the change
        watcher->collisionDetector->handleFileChange(filename, events);
    }
}
