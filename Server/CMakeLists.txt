cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)

# FetchContent for handling dependencies
include(FetchContent)

# Fetch libpqxx
FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG master
)
FetchContent_MakeAvailable(libpqxx)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(json)

# Find PostgreSQL
find_package(PostgreSQL REQUIRED)

# Include directories
include_directories(include)
include_directories(${libpqxx_SOURCE_DIR}/include)
include_directories(${PostgreSQL_INCLUDE_DIRS})
include_directories(${nlohmann_json_INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE SERVER_SOURCES "src/*.cpp")

# Server Executable
add_executable(Server ${SERVER_SOURCES}
        src/main.cpp
        include/ClientCommunicator.h
        src/ClientCommunicator.cpp
        include/Database/DatabaseManager.h
        src/Database/DatabseManager.cpp
)

# Link libraries
target_link_libraries(Server
        pqxx
        nlohmann_json::nlohmann_json
        ${PostgreSQL_LIBRARIES}
        # e.g., OpenSSL, etc.
)
