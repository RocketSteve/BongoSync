cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)

# Find PostgreSQL and libpqxx
find_package(PostgreSQL REQUIRED)
#

set(LIBPQXX_INCLUDE_DIR "/opt/homebrew/Cellar/libpqxx/7.8.1/include")
set(LIBPQXX_LIBRARY "/opt/homebrew/Cellar/libpqxx/7.8.1/lib/libpqxx.dylib")

# Add JSON library directory
add_subdirectory(lib/json)
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
        include/Database/FilesManager.h
        src/Database/FilesManager.cpp
        include/Handlers/RegistrationHandler.h
        src/Handlers/RegistrationHandler.cpp
        src/Database/DatabseManager.cpp
        src/Database/DatabseManager.cpp
        include/Handlers/HandleLogin.h
        src/Handlers/HandleLogin.cpp
)

# Include directories
target_include_directories(Server PRIVATE
        "/opt/homebrew/include"
        ${PostgreSQL_INCLUDE_DIRS}
        include
        ${nlohmann_json_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(Server
        "/opt/homebrew/lib/libpqxx.dylib"
        nlohmann_json::nlohmann_json
        ${PostgreSQL_LIBRARIES}
        # Add other libraries if needed
)
