cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)
# Include other necessary directories

# Source files
file(GLOB_RECURSE SERVER_SOURCES "src/*.cpp")

# Server Executable
add_executable(Server ${SERVER_SOURCES})

# Link libraries
target_link_libraries(Server
        # Link necessary libraries specific to the server
        # e.g., pqxx, OpenSSL, etc.
)
