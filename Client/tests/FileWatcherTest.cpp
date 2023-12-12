#define BOOST_TEST_MODULE FileWatcherTest
#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>
#include "FileWatcher.h"
#include <fstream>
#include <thread>

// Utility function to create a dummy file
void createDummyFile(const std::string& path) {
    std::ofstream file(path);
    file << "Test content";
    file.close();
}

// Utility function to modify a file
void modifyDummyFile(const std::string& path) {
    std::ofstream file(path, std::ios_base::app);
    file << " More content";
    file.close();
}

// Utility function to delete a file
void deleteDummyFile(const std::string& path) {
    boost::filesystem::remove(path);
}

struct FileWatcherFixture {
    FileWatcherFixture() {
        // Setup code before each test case
    }

    ~FileWatcherFixture() {
        // Teardown code after each test case
    }

    // You can add common utilities or member variables here
};

// Test Case: Detecting File Creation
BOOST_FIXTURE_TEST_CASE(test_file_creation, FileWatcherFixture) {
bool fileCreated = false;
std::string testFilePath = "testfile.txt";

FileWatcher watcher(".", [&](const std::string& filename) {
    if (filename == testFilePath) {
        fileCreated = true;
    }
});

watcher.start();
createDummyFile(testFilePath);
std::this_thread::sleep_for(std::chrono::seconds(2)); // Allow time for detection
watcher.stop();

BOOST_CHECK(fileCreated);
}

// Test Case: Detecting File Modification
BOOST_FIXTURE_TEST_CASE(test_file_modification, FileWatcherFixture) {
bool fileModified = false;
std::string testFilePath = "testfile.txt";

FileWatcher watcher(".", [&](const std::string& filename) {
    if (filename == testFilePath) {
        fileModified = true;
    }
});

createDummyFile(testFilePath);
watcher.start();
modifyDummyFile(testFilePath);
std::this_thread::sleep_for(std::chrono::seconds(2)); // Allow time for detection
watcher.stop();

BOOST_CHECK(fileModified);
}

// Test Case: Detecting File Deletion
BOOST_FIXTURE_TEST_CASE(test_file_deletion, FileWatcherFixture) {
bool fileDeleted = false;
std::string testFilePath = "testfile.txt";

createDummyFile(testFilePath);

FileWatcher watcher(".", [&](const std::string& filename) {
    if (filename == testFilePath) {
        fileDeleted = true;
    }
});

watcher.start();
deleteDummyFile(testFilePath);
std::this_thread::sleep_for(std::chrono::seconds(2)); // Allow time for detection
watcher.stop();

BOOST_CHECK(fileDeleted);
}
