cmake_minimum_required(VERSION 3.5)
project(weather_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CPR_CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(FTXUI_BUILD_DOCS OFF CACHE INTERNAL "")
set(FTXUI_BUILD_TESTS OFF CACHE INTERNAL "")
set(CPR_BUILD_TESTS OFF CACHE INTERNAL "")
set(JSON_BuildTests OFF CACHE INTERNAL "")

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.14.2)

FetchContent_Declare(ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git GIT_TAG v7.0.0)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.15.1)

FetchContent_MakeAvailable(json cpr ftxui Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

enable_testing()

add_subdirectory(app)
add_subdirectory(test)
add_subdirectory(core)
