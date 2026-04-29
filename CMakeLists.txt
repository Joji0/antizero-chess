cmake_minimum_required(VERSION 3.16)
project(antizero-chess LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# TODO: this optimization does not work on Windows, need to config that somehow in the future

add_executable(engine
        src/main.cpp
        src/bitboard.cpp
        src/position.cpp
)

add_executable(tests
        tests/test_move.cpp
        src/position.cpp
        src/bitboard.cpp
)
