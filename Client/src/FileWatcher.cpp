#include "../include/FileWatcher.h"
#include <iostream>

FileWatcher::FileWatcher(const std::string& path, std::function<void(const std::string&)> onChange)
        : directoryToWatch(path), onChangeCallback(onChange), running(false) {
    loop = uv_default_loop();
    uv_fs_event_init(loop, &fsEvent);
}

FileWatcher::~FileWatcher() {
    stop();
}

void FileWatcher::start() {
    running = true;
    watcherThread = std::thread([this]() {
        fsEvent.data = this;
        uv_fs_event_start(&fsEvent, FileWatcher::onFileChange, directoryToWatch.c_str(), 0);
        uv_run(loop, UV_RUN_DEFAULT);
    });
}

void FileWatcher::stop() {
    if (running) {
        running = false;
        uv_stop(loop);

        if (watcherThread.joinable()) {
            watcherThread.join();
        }
        uv_fs_event_stop(&fsEvent);
        uv_loop_close(loop);
    }
}

void FileWatcher::onFileChange(uv_fs_event_t* handle, const char* filename, int events, int status) {
    FileWatcher* watcher = static_cast<FileWatcher*>(handle->data);
    if (watcher && filename) {
        watcher->onChangeCallback(filename);
    }
}

// Sample usage
/*
int main() {
    FileWatcher watcher("/path/to/directory", [](const std::string& filename) {
        std::cout << "File changed: " << filename << std::endl;
    });

    watcher.start();
    // Do other work
    watcher.stop();
    return 0;
}
*/
