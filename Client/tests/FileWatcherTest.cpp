#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <map>
#include <thread>
#include "../include/fileServices/FileWatcher.h"
#include "../include/fileServices/FileChangeDetector.h"
#include "MockFileChangeDetector.cpp"

using ::testing::AtLeast; // or other matchers as needed

class FileWatcherTest : public ::testing::Test {
protected:
    MockFileChangeDetector* mockDetector;
    FileWatcher* watcher;

    std::string testDirectory = "../tests/test_data";

    void SetUp() override {
        mockDetector = new MockFileChangeDetector();
        watcher = new FileWatcher(mockDetector);

        // Setup expectations on the mock object
        EXPECT_CALL(*mockDetector, detectChanges())
                .Times(AtLeast(1)); // Expect the method to be called at least once
    }

    void TearDown() override {
        delete watcher;
        delete mockDetector;
    }

    void simulateFileCreation(const std::string& filename) {
        std::ofstream file(testDirectory + "/" + filename);
        if (file.is_open()) {
            file << "Test content";
            file.close();
        } else {
            std::cout << "Error opening file: " << filename << std::endl;
        }
    }


    void simulateFileModification(const std::string& filename) {
        std::ofstream file(testDirectory + "/" + filename, std::ios::app); // Append mode
        if (file.is_open()) {
            file << "\nAdditional content"; // Append content to the file
            file.close();
        } else {
            std::cout << "Error opening file: " << filename << std::endl;
        }
    }


    void simulateFileDeletion(const std::string& filename) {
        if (std::filesystem::remove(testDirectory + "/" + filename) == false) {
            std::cout << "Error deleting file: " << filename << std::endl;
        }
    }

};

TEST_F(FileWatcherTest, DetectsFileCreation) {
    std::string filename = "testCreate.txt";
    simulateFileCreation(filename);

    // Assuming FileWatcher has a method to clear its detected changes
    watcher->clearDetectedChanges();

    watcher->start();

    // Wait for the file system event to be captured
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Assuming FileWatcher has a method to check if a file creation was detected
    ASSERT_TRUE(watcher->wasFileCreated(filename));

    // Verify that the mock's detectChanges method was called
    ::testing::Mock::VerifyAndClearExpectations(mockDetector);

    // Clean up
    simulateFileDeletion(filename);
}
