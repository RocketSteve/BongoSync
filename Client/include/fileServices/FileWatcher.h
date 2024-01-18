#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <uv.h>
#include <string>
#include <iostream>

class FileWatcher {
public:
    // Constructor and Destructor
    FileWatcher();
    ~FileWatcher();

    // Disallow copy constructor and assignment operator
    FileWatcher(const FileWatcher&) = delete;
    FileWatcher& operator=(const FileWatcher&) = delete;

    // Initialize the file watcher
    void initialize(const std::string& directoryPath);

    // Get the singleton instance of the file watcher
    static FileWatcher& getInstance();

    // Start and stop the file watcher
    void start();
    void stop();

private:
    // Libuv loop and file system event
    uv_loop_t* loop;
    uv_fs_event_t fsEvent;

    // Directory to watch
    std::string directoryToWatch;

    // Running state
    bool isRunning;

    // Static callback for libuv file system event changes
    static void onFileChange(uv_fs_event_t* handle, const char* filename, int events, int status);
};

#endif // FILEWATCHER_H
