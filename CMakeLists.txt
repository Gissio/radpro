cmake_minimum_required(VERSION 3.16)

project(radpro)

set(CMAKE_C_STANDARD 99)

FILE(GLOB sources platform.io/src/*.c)
FILE(GLOB sdl_sources platform.io/src/sdl/*.c)
FILE(GLOB mcurenderer_sources platform.io/lib/mcu-renderer/*.c)
FILE(GLOB mcumax_sources platform.io/lib/mcu-max/*.c)

include_directories(platform.io/lib/mcu-renderer platform.io/lib/mcu-max ${SERCOMM_INCLUDE_DIR})

add_definitions(-D SIMULATOR)
add_definitions(-D SIMULATE_PULSES)
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_definitions(-D SIMULATE_COMM)
endif()
add_definitions(-D TUBE_HV_PWM)
# add_definitions(-D BATTERY_REMOVABLE)
# add_definitions(-D KEYBOARD_2KEYS)
# add_definitions(-D KEYBOARD_3KEYS)
add_definitions(-D KEYBOARD_5KEYS)
add_definitions(-D KEYBOARD_POWERON_NOWAIT)
add_definitions(-D VIBRATION)
add_definitions(-D PULSE_LED)
# add_definitions(-D ALERT_LED)
# add_definitions(-D DATA_MODE)
add_definitions(-D GAME)
add_definitions(-D MCURENDERER_SDL)
# add_definitions(-D ST7789_DEBUG)
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_compile_options(-O3 -sUSE_SDL=2)
    link_libraries(SDL2)
else()
    add_link_options(/NODEFAULTLIB:libcmt /NODEFAULTLIB:libcmtd)
endif()

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    find_package(SDL2 CONFIG REQUIRED)
    find_path(SERCOMM_INCLUDE_DIR sercomm/sercomm.h)
    find_library(SERCOMM_LIB sercomm.lib)

    link_libraries($<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        ${SERCOMM_LIB}
    )
endif()

add_executable(radpro-monochrome ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-monochrome PUBLIC DISPLAY_128X64 DISPLAY_MONOCHROME)

add_executable(radpro-color-landscape ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-landscape PUBLIC DISPLAY_320X240 DISPLAY_COLOR)

add_executable(radpro-color-portrait ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-portrait PUBLIC DISPLAY_240X320 DISPLAY_COLOR)
