cmake_minimum_required(VERSION 3.26)
project(simpleDatabase)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.0.0
)

FetchContent_MakeAvailable(fmt)

add_executable(simpleDatabase main.cpp
        database.cpp
        queryParser.cpp
        instructions.h
        instructions.cpp
)
target_link_libraries(simpleDatabase fmt)
