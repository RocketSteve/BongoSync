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
        // Use FileChangeDetector to detect changes
        auto changes = watcher->fileChangeDetector->detectChanges();

        // Process the detected changes
        for (const auto& change : changes) {
            // Here, you can process each change as needed.
            // For example, you might log the change, update a user interface, or trigger some other action.
            std::cout << "Change detected: " << change.getFilePath() << std::endl;

            // Implement additional logic based on the type of change
            switch (change.getChangeType()) {
                case FileChange::Type::Added:
                    std::cout << "File added: " << change.getFilePath() << std::endl;
                    break;
                case FileChange::Type::Deleted:
                    std::cout << "File deleted: " << change.getFilePath() << std::endl;
                    break;
                case FileChange::Type::Modified:
                    std::cout << "File modified: " << change.getFilePath() << std::endl;
                    break;
            }
        }
    }
}



