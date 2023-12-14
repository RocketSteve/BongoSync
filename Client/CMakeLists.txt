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
        include/FileWatcher.h
        src/FileWatcher.cpp
        include/HashCalculator.h
        src/HashCalculator.cpp
        include/HandleRegister.h
        src/HandleRegister.cpp
        include/HandleStart.h
        include/HandleSync.h
        include/HandleShare.h
        include/HandleUnshare.h
        src/HandleStart.cpp
        src/HandleSync.cpp
        src/HandleShare.cpp
        src/HandleUnshare.cpp
        include/HandleBreak.h
        include/ServerCommunicator.h
        src/ServerCommunicator.cpp
        include/MessageBuilder.h
        src/MessageBuilder.cpp
        src/Utility.cpp
        include/MessageCreator.h
        include/CollisionDetector.h
        src/HandleBreak.cpp
        src/CollisionDetector.cpp
)

# Link libraries
target_link_libraries(Client
        ${OPENSSL_LIBRARIES}
        nlohmann_json::nlohmann_json
        uv
        /home/simoszy/Documents/BongoSync/lib/BLAKE3/c/build/libblake3.a
        # Link any other necessary libraries here
)
