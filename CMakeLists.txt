cmake_minimum_required(VERSION 3.10)
project(SpotifyApp)

set(CMAKE_CXX_STANDARD 17)
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(SpotifyApp main.cpp)

target_link_libraries(SpotifyApp PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
