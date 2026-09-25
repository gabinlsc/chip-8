cmake_minimum_required(VERSION 3.16)
project(Chip8Emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Téléchargement et compilation automatique de Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

include_directories(include)

add_executable(chip8_emu 
    src/main.cpp 
    src/chip8.cpp
)

target_link_libraries(chip8_emu PRIVATE raylib)