cmake_minimum_required(VERSION 3.0.0)
project(fs2011pro VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)

FILE(GLOB sources ../cubeide/Core/fs2011pro/*.c)
FILE(GLOB u8g2Sources ../cubeide/Core/fs2011pro/u8g2/*.c)
FILE(GLOB mcumaxSources ../cubeide/Core/fs2011pro/mcu-max/*.c)

add_definitions(-DSDL_MODE)

add_executable(fs2011pro main.c sdl/u8x8_d_sdl_128x64.c sdl/u8x8_sdl_key.c ${sources} ${u8g2Sources} ${mcumaxSources})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(fs2011pro PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(fs2011pro PRIVATE ../cubeide/Core/fs2011pro/u8g2)
