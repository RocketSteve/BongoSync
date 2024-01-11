cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# Specify OpenSSL paths
set(OPENSSL_INCLUDE_DIR /opt/homebrew/Cellar/openssl@3/3.2.0_1/include)
set(OPENSSL_SSL_LIBRARY /opt/homebrew/Cellar/openssl@3/3.2.0_1/lib/libssl.dylib)
set(OPENSSL_CRYPTO_LIBRARY /opt/homebrew/Cellar/openssl@3/3.2.0_1/lib/libcrypto.dylib)
include_directories(${OPENSSL_INCLUDE_DIR})


FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(json)

# Fetch GTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)


# Fetch BLAKE3
FetchContent_Declare(
        blake3
        GIT_REPOSITORY https://github.com/BLAKE3-team/BLAKE3.git
        GIT_TAG master
)
FetchContent_MakeAvailable(blake3)

FetchContent_Declare(
        libuv
        GIT_REPOSITORY https://github.com/libuv/libuv.git
        GIT_TAG v1.47.0
)
FetchContent_MakeAvailable(libuv)




# Include directories
include_directories(include)
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${blake3_SOURCE_DIR}/c) # Assuming this is the correct path
# Include other necessary directories

# Source files
set(CLIENT_SOURCES
        src/main.cpp
        src/Utility.cpp
        src/commandHandlers/HandleBreak.cpp
        src/commandHandlers/HandleRegister.cpp
        src/commandHandlers/HandleShare.cpp
        src/commandHandlers/HandleStart.cpp
        src/commandHandlers/HandleSync.cpp
        src/commandHandlers/HandleUnshare.cpp
        src/communication/MessageBuilder.cpp
        src/communication/ServerCommunicator.cpp
        src/fileServices/ConflictResolver.cpp
        src/fileServices/FileChangeDetector.cpp
        src/fileServices/FileWatcher.cpp
        src/fileServices/HashCalculator.cpp
        src/fileServices/MerkleTree.cpp
        # Add more source files here
)


# Client Executable
add_executable(Client src/main.cpp ${CLIENT_SOURCES})

# Link libraries
target_link_libraries(Client
        ${OPENSSL_SSL_LIBRARY}
        ${OPENSSL_CRYPTO_LIBRARY}
        nlohmann_json::nlohmann_json
        blake3 # Assuming this is the correct target name
        uv
        gtest
        gtest_main
        pthread
        # Link any other necessary libraries here
)

# Enable testing
enable_testing()

# Test Executable
add_executable(ClientTests
        tests/test.cpp
        tests/MerkleTreeTest.cpp
        ${CLIENT_SOURCES}
        tests/MessageBuilderTest.cpp
)

# Link libraries for Test Executable
target_link_libraries(ClientTests
        ${OPENSSL_SSL_LIBRARY}
        ${OPENSSL_CRYPTO_LIBRARY}
        nlohmann_json::nlohmann_json
        blake3 # Assuming this is the correct target name
        uv
        gtest_main
        gmock_main # And this line
        pthread
        # Add other dependencies used by your tests
)

# Add tests
gtest_discover_tests(ClientTests)
add_executable(server ../Server/include/Database/Database.h
        ../Server/src/Database/Database.cpp)
