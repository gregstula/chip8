cmake_minimum_required(VERSION 3.30)
project(chip8)

set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Threads REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

set (SRCS
    chip8.cpp
    chip8.hpp
    display.cpp
    display.hpp
    keypad.hpp
    main.cpp
)

if(MSVC)
    add_compile_options("/W4" "$<$<CONFIG:Release>:/O2>")
else()
    add_compile_options("-Wall" "-Wextra" "-pedantic" "$<$<CONFIG:Release>:-O2>")
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_compile_features(chip8 PRIVATE cxx_std_23)
set_target_properties(chip8 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(chip8 PRIVATE SDL2::SDL2 Threads::Threads)

