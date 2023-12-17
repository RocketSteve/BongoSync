#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <uv.h>
#include <string>
#include "FileChangeDetector.h"

class FileWatcher {
public:
    explicit FileWatcher(const std::string& directoryPath);
    ~FileWatcher();

    static FileWatcher& getInstance();
    void initialize(const std::string& directoryPath);
    FileWatcher(const FileWatcher&) = delete; // Delete copy constructor
    FileWatcher& operator=(const FileWatcher&) = delete; // Delete copy assignment operator

    void start();
    void stop();

private:
    FileWatcher(); // Private default constructor
    static void onFileChange(uv_fs_event_t* handle, const char* filename, int events, int status);

    uv_loop_t* loop;
    uv_fs_event_t fsEvent;
    std::string directoryToWatch;
    FileChangeDetector *fileChangeDetector; // Now initialized in the constructor
    bool isRunning;
};

#endif // FILEWATCHER_H
