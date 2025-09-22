cmake_minimum_required(VERSION 3.30)
project(chip8)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core )
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_standard_project_setup()


include_directories(${SDL2_INCLUDE_DIRS})

set (SRCS
    chip8.cpp
    chip8.hpp
    keypad.hpp
    display.hpp
    display.cpp
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
target_link_libraries(chip8 PRIVATE
    SDL2::SDL2
    Threads::Threads
    Qt6::Core
    Qt6::Widgets
)

