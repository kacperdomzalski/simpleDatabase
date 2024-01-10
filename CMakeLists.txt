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

add_executable(simpleDatabase src/main.cpp
        src/database.cpp
        src/queryParser.cpp
        src/headers/instructions.h
        src/instructions.cpp
        src/headers/table.h
        src/table.cpp
        src/headers/config.h
        src/headers/select.h
        src/select.cpp
        src/headers/memory.h
        src/memory.cpp
        src/headers/whereFilter.h
        src/whereFilter.cpp
        src/headers/update.h
        src/update.cpp
        src/updateSetFilter.cpp
        src/headers/deleteRow.h
        src/deleteRow.cpp
)
target_link_libraries(simpleDatabase fmt)
