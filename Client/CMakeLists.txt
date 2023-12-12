cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

add_executable(client src/main.cpp)


add_executable(Client src/main.cpp
        include/FileWatcher.h
        src/FileWatcher.cpp
        tests/FileWatcherTest.cpp
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
)
