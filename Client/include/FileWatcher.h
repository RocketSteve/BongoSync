#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <uv.h>
#include <string>
#include <functional>
#include <thread>
#include <atomic>

class FileWatcher {
public:
    FileWatcher(const std::string& path, std::function<void(const std::string&)> onChange);
    ~FileWatcher();

    void start();
    void stop();

private:
    static void onFileChange(uv_fs_event_t* handle, const char* filename, int events, int status);

    uv_fs_event_t fsEvent;
    uv_loop_t* loop;
    std::string directoryToWatch;
    std::function<void(const std::string&)> onChangeCallback;
    std::thread watcherThread;
    std::atomic<bool> running;
};

#endif // FILEWATCHER_H
