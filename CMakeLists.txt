# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.19)
project(chip8)
set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

set (SRCS
    chip8.cpp
    chip8.hpp
    display.cpp
    display.hpp
    main.cpp
)

if(MSVC)
    add_compile_options("/W4" "$<$<CONFIG:Release>:/02>")
else()
    add_compile_options("-Wall" "-Wextra" "-pedantic" "$<$<CONFIG:Debug>:-fsanitize=address>" "$<$<CONFIG:Release>:-02>")
    add_link_options("$<$<CONFIG:Debug>:-fsanitize=address>")
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_compile_features(chip8 PUBLIC cxx_std_17)
set_target_properties(chip8 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(chip8 PRIVATE SDL2::SDL2)

