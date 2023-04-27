cmake_minimum_required(VERSION 3.0.0)

project(fs2011pro)

set(CMAKE_C_STANDARD 99)

FILE(GLOB sources platform.io/src/*.c)
FILE(GLOB U8g2Sources platform.io/.pio/libdeps/fs2011stm32/U8g2/src/clib/*.c)
FILE(GLOB mcumaxSources platform.io/lib/mcu-max/*.c)

add_definitions(-DSDL_MODE)
add_definitions(-DSIMULATE_PULSES)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(fs2011pro ${sources} ${U8g2Sources} ${mcumaxSources})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(fs2011pro PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(fs2011pro PRIVATE platform.io/.pio/libdeps/fs2011stm32/U8g2/src platform.io/lib/mcu-max)
