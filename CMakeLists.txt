cmake_minimum_required(VERSION 3.1.0)

project(listener VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

include(FetchContent)

FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt GIT_TAG e737672614dfad3a6df23ffe3f2348fcfa4d3944)
FetchContent_MakeAvailable(fmt)

add_executable(listener listener.cpp)

target_link_libraries(listener fmt::fmt)
