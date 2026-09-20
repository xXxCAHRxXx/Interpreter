cmake_minimum_required(VERSION 3.14)

project(
    Interpreter
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_Declare(
    GSL
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG v4.2.1
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(json GSL)

add_subdirectory(lib)
add_subdirectory(bin)