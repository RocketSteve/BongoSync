cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(GTest REQUIRED) # Add this line to find GTest

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
        ${OPENSSL_LIBRARIES}
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
# Link libraries for Test Executable
target_link_libraries(ClientTests
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        pthread
        ${OPENSSL_LIBRARIES}
        nlohmann_json::nlohmann_json
        uv
        /home/simoszy/Documents/BongoSync/lib/BLAKE3/c/build/libblake3.a
        # Add other dependencies used by your tests
)

# Add tests
gtest_discover_tests(ClientTests)

