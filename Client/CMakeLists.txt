cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
include_directories(/opt/homebrew/include)  # Add this line to include headers
link_directories(/opt/homebrew/lib)
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

find_package(OpenSSL REQUIRED)



# Include directories
include_directories(include)
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
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
        tests/MessageBuilderTest.cpp
)

# Link libraries for Test Executable
target_link_libraries(ClientTests
        OpenSSL::SSL
        OpenSSL::Crypto
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
add_executable(server ../Server/include/Database.h
        ../Server/src/Database.cpp)
