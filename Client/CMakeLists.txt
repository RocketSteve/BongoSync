cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

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

# Fetch OpenSSL
FetchContent_Declare(
        openssl
        GIT_REPOSITORY https://github.com/openssl/openssl.git
        GIT_TAG OpenSSL_1_1_1k # Replace with the desired OpenSSL version
)
FetchContent_GetProperties(openssl)
if(NOT openssl_POPULATED)
    FetchContent_Populate(openssl)
    add_subdirectory(${openssl_SOURCE_DIR} ${openssl_BINARY_DIR})
endif()

# Include directories
include_directories(include)
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(/usr/local/include)
include_directories(/usr/include/gtest)
include_directories(/home/simoszy/Documents/BongoSync/lib/BLAKE3/c)
link_directories(/usr/lib)

# Source files
file(GLOB_RECURSE CLIENT_SOURCES "src/*.cpp")
list(REMOVE_ITEM CLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Client Executable
add_executable(Client src/main.cpp ${CLIENT_SOURCES})

# Link libraries
target_link_libraries(Client
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        uv
        /home/simoszy/Documents/BongoSync/lib/BLAKE3/c/build/libblake3.a
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
        ${CLIENT_SOURCES} # Add this if tests require the main application's sources
)

target_link_libraries(Client
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        uv
        /home/simoszy/Documents/BongoSync/lib/BLAKE3/c/build/libblake3.a
        gtest
        gtest_main
        pthread
        # Add other dependencies used by your tests
)

# Add tests
gtest_discover_tests(ClientTests)

