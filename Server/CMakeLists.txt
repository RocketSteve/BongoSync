cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find PostgreSQL and libpqxx
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/googletest/1.14.0")


if(NOT PQXX_FOUND)
    message(FATAL_ERROR "libpqxx not found")
endif()
pkg_check_modules(BLAKE3 REQUIRED IMPORTED_TARGET libblake3)
if(NOT BLAKE3_FOUND)
    message(FATAL_ERROR "blake3 not found")
endif()
pkg_check_modules(GTEST REQUIRED IMPORTED_TARGET gtest_main)

if(NOT GTEST_FOUND)
    message(FATAL_ERROR "gtest not found")
endif()

# Find nlohmann/json
find_package(nlohmann_json REQUIRED)



# Include the directories of the libraries
include_directories(${PQXX_INCLUDE_DIRS} ${BLAKE3_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

# Add JSON library directory
# add_subdirectory(lib/json)  # You can remove this line if you're using find_package for nlohmann_json

file(GLOB_RECURSE CLIENT_SOURCES "src/*.cpp")

# Add executable
add_executable(Server
        ${CLIENT_SOURCES}
        src/main.cpp
        include/Communciation/MessageBuilder.h
        include/Communciation/MessageCreator.h
        src/Communication/MessageBuilder.cpp
        include/Database/UserManager.h
        src/Database/UserManager.cpp
        include/Database/WorkspaceManager.h
        src/Database/WorkspaceManager.cpp
        include/Database/SharedManager.h
        src/Database/SharedManager.cpp
        src/FileServices/MerkleTreeComparer.cpp
)

# Link the libraries to your executable
target_link_libraries(Server PUBLIC PkgConfig::PQXX PkgConfig::BLAKE3 nlohmann_json::nlohmann_json)

add_executable(ServerTest
        tests/MerkleTreeComparerTest.cpp
    src/FileServices/MerkleTreeComparer.cpp
    include/FileServices/MerkleTreeComparer.h
        src/FileServices/MerkleTree.cpp
    include/FileServices/MerkleTree.h
        include/FileServices/HashCalculator.h
        src/FileServices/HashCalculator.cpp
        tests/MerkleTreeTest.cpp)


target_link_libraries(ServerTest PUBLIC PkgConfig::PQXX PkgConfig::BLAKE3 nlohmann_json::nlohmann_json PkgConfig::GTEST)
