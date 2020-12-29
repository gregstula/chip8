cmake_minimum_required(VERSION 3.8)
project(chip8Emu VERSION 0.1 DESCRIPTION "A chip-8 emulator with SDL" LANGUAGES CXX)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(chip8 main.cpp display.cpp chip8.cpp display.hpp chip8.hpp)
target_link_libraries(chip8 PRIVATE SDL2::SDL2)
target_compile_features(chip8 PUBLIC cxx_std_17)
# disable things like -std=g++17
set_target_properties(chip8 PROPERTIES CXX_EXTENSIONS OFF)

