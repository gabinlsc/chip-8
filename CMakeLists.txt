cmake_minimum_required(VERSION 3.16)
project(Chip8Emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(raylib REQUIRED)

add_executable(chip8_emu 
    src/main.cpp 
    src/chip8.cpp
)

target_link_libraries(chip8_emu PRIVATE raylib)