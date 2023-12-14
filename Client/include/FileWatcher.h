#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <uv.h>
#include <string>
#include <functional>
#include "../include/CollisionDetector.h"

class CollisionDetector;  // Forward declaration

class FileWatcher {
public:
    static FileWatcher& getInstance();
    FileWatcher(const FileWatcher&) = delete;            // Delete copy constructor
    FileWatcher& operator=(const FileWatcher&) = delete; // Delete copy assignment operator

    void start(const std::string& path, CollisionDetector& collisionDetector);
    void stop();

private:
    FileWatcher();  // Private constructor
    ~FileWatcher(); // Destructor

    static void onFileChange(uv_fs_event_t* handle, const char* filename, int events, int status);

    uv_loop_t* loop;
    uv_fs_event_t fsEvent;
    std::string directoryToWatch;
    CollisionDetector* collisionDetector;
    bool isRunning;
};

#endif // FILEWATCHER_H
