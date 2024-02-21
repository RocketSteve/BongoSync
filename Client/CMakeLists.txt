cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
if(NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found")
endif()

find_library(LIBUV_LIBRARY NAMES uv)

if(NOT LIBUV_LIBRARY)
    message(FATAL_ERROR "libuv not found")
endif()

# Find nlohmann/json
find_package(nlohmann_json REQUIRED)


# # Find BLAKE
# pkg_check_modules(BLAKE3 REQUIRED IMPORTED_TARGET libblake3)
# if(NOT BLAKE3_FOUND)
#     message(FATAL_ERROR "blake3 not found")
# endif()

# Include the directories of the libraries
include_directories(${OPENSSL_INCLUDE_DIR} ${nlohmann_json_INCLUDE_DIRS} ${GTest_INCLUDE_DIRS} ${BLAKE3_INCLUDE_DIRS})

file(GLOB_RECURSE CLIENT_SOURCES "src/*.cpp")

list(REMOVE_ITEM CLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main_main.cpp")

# Add executable
add_executable(Client
        ${CLIENT_SOURCES}
)

# Link the libraries to your executable
# target_link_libraries(Client PUBLIC OpenSSL::SSL OpenSSL::Crypto ${LIBUV_LIBRARY} nlohmann_json::nlohmann_json PkgConfig::BLAKE3)
target_link_libraries(Client PUBLIC OpenSSL::SSL OpenSSL::Crypto ${LIBUV_LIBRARY} nlohmann_json::nlohmann_json)
