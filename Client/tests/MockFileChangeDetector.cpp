#include <gmock/gmock.h>
#include "../include/fileServices/FileChangeDetector.h"

class MockFileChangeDetector : public FileChangeDetector {
public:
    MOCK_METHOD(std::vector<FileChange>, detectChanges, (), (override));
};
