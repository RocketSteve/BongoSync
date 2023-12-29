cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch nlohmann_json
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
        GIT_TAG OpenSSL_1_1_1k
)
FetchContent_MakeAvailable(openssl)

# Fetch BLAKE3
FetchContent_Declare(
        blake3
        GIT_REPOSITORY https://github.com/BLAKE3-team/BLAKE3.git
        GIT_TAG some_specific_tag_or_branch # Replace with a specific tag or branch
)
FetchContent_MakeAvailable(blake3)

# Include directories
include_directories(include)
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${openssl_SOURCE_DIR}/include)
include_directories(${blake3_SOURCE_DIR}/c) # Assuming this is the correct path
# Include other necessary directories

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
)

# Link libraries for Test Executable
target_link_libraries(ClientTests
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        blake3 # Assuming this is the correct target name
        uv
        gtest
        gtest_main
        pthread
        # Add other dependencies used by your tests
)

# Add tests
gtest_discover_tests(ClientTests)
