cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${nlohmann_json_INCLUDE_DIRS})

include_directories(/usr/local/include)


include_directories(/home/simoszy/Documents/BongoSync/lib/BLAKE3/c)


add_executable(Client src/main.cpp
        include/fileServices/FileWatcher.h
        src/fileServices/FileWatcher.cpp
        include/fileServices/HashCalculator.h
        src/fileServices/HashCalculator.cpp
        include/commandHandlers/HandleRegister.h
        src/commandHandlers/HandleRegister.cpp
        include/commandHandlers/HandleStart.h
        include/commandHandlers/HandleSync.h
        include/commandHandlers/HandleShare.h
        include/commandHandlers/HandleUnshare.h
        src/commandHandlers/HandleStart.cpp
        src/commandHandlers/HandleSync.cpp
        src/commandHandlers/HandleShare.cpp
        src/commandHandlers/HandleUnshare.cpp
        include/commandHandlers/HandleBreak.h
        include/communication/ServerCommunicator.h
        src/communication/ServerCommunicator.cpp
        include/communication/MessageBuilder.h
        src/communication/MessageBuilder.cpp
        src/Utility.cpp
        include/communication/MessageCreator.h
        src/commandHandlers/HandleBreak.cpp
        include/fileServices/MerkleTree.h
        src/fileServices/MerkleTree.cpp
        include/fileServices/FileChangeDetector.h
        src/fileServices/FileChangeDetector.cpp
        include/fileServices/ConflictResolver.h
        src/fileServices/ConflictResolver.cpp
        include/fileServices/FileServices.h
        src/fileServices/FileServices.cpp
)

# Link libraries
target_link_libraries(Client
        ${OPENSSL_LIBRARIES}
        nlohmann_json::nlohmann_json
        uv
        /home/simoszy/Documents/BongoSync/lib/BLAKE3/c/build/libblake3.a
        # Link any other necessary libraries here
)
