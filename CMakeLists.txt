cmake_minimum_required(VERSION 3.10)

# Set project name
project(DeribitTradingSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find required packages
find_package(CURL REQUIRED)

# Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/nlohmann)
include_directories(${CMAKE_SOURCE_DIR}/third_party/boost)
include_directories(${OPENSSL_INCLUDE_DIR})
find_package(OpenSSL REQUIRED)


# Source files
set(SOURCES
    src/main.cpp
    src/auth.cpp
    src/order_manager.cpp
    src/websocket_server.cpp
    src/utils.cpp
)

# Add executable target
add_executable(DeribitTradingSystem ${SOURCES})

# Link libraries
target_link_libraries(DeribitTradingSystem PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(DeribitTradingSystem PRIVATE CURL::libcurl)

