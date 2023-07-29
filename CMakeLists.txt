cmake_minimum_required(VERSION 3.0.0)

project(radpro)

set(CMAKE_C_STANDARD 99)

FILE(GLOB sources platform.io/src/*.c)
FILE(GLOB io_sources platform.io/src/devices/io/*.c)
FILE(GLOB sdlsim_sources platform.io/src/devices/sdlsim/*.c)
FILE(GLOB U8g2Sources platform.io/.pio/libdeps/fs2011-stm32f051c8/U8g2/src/clib/*.c)
FILE(GLOB mcumaxSources platform.io/lib/mcu-max/*.c)

add_definitions(-DSDLSIM)
add_definitions(-DDISPLAY_U8G2)
add_definitions(-DDISPLAY_128X64)
add_definitions(-DKEYBOARD_5KEYS)
add_definitions(-DSIMULATE_PULSES)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(radpro ${sources} ${io_sources} ${sdlsim_sources} ${U8g2Sources} ${mcumaxSources})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(radpro PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(radpro PRIVATE platform.io/.pio/libdeps/fs2011-stm32f051c8/U8g2/src platform.io/lib/mcu-max)
