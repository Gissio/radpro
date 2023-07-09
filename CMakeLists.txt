cmake_minimum_required(VERSION 3.0.0)

project(radpro)

set(CMAKE_C_STANDARD 99)

FILE(GLOB sources platform.io/src/*.c)
FILE(GLOB common_sources platform.io/src/devices/common/*.c)
FILE(GLOB sdlsim_sources platform.io/src/devices/sdlsim/*.c)
FILE(GLOB U8g2Sources platform.io/.pio/libdeps/fs2011/U8g2/src/clib/*.c)
FILE(GLOB mcumaxSources platform.io/lib/mcu-max/*.c)

add_definitions(-D SDLSIM)
add_definitions(-D DISPLAY_U8G2)
add_definitions(-D DISPLAY_128X64)
add_definitions(-D KEYBOARD_5KEYS)
add_definitions(-D SIMULATE_PULSES)
add_definitions(-D _CRT_SECURE_NO_WARNINGS)

add_executable(radpro ${sources} ${common_sources} ${sdlsim_sources} ${U8g2Sources} ${mcumaxSources})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(radpro PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(radpro PRIVATE platform.io/.pio/libdeps/fs2011/U8g2/src platform.io/lib/mcu-max)
