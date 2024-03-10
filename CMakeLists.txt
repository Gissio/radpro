cmake_minimum_required(VERSION 3.16)

project(radpro)

set(CMAKE_C_STANDARD 99)
set(VCPKG_TARGET_TRIPLET x64-windows-static)

FILE(GLOB sources platform.io/src/*.c)
FILE(GLOB sdl_sources platform.io/src/sdl/*.c)
FILE(GLOB mcurenderer_sources platform.io/lib/mcu-renderer/*.c)
FILE(GLOB mcumax_sources platform.io/lib/mcu-max/*.c)

add_definitions(-D SIMULATOR)
add_definitions(-D SIMULATE_PULSES)
add_definitions(-D BATTERY_REMOVABLE)
add_definitions(-D KEYBOARD_5KEYS)
add_definitions(-D PULSE_LED)
add_definitions(-D MCURENDERER_SDL)
add_definitions(-D _CRT_SECURE_NO_WARNINGS)

find_package(SDL2 CONFIG REQUIRED)
find_path(SERCOMM_INCLUDE_DIR sercomm/sercomm.h)
find_library(SERCOMM_LIB sercomm.lib)

add_executable(radpro-monochrome ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-monochrome PUBLIC DISPLAY_128X64)
target_compile_definitions(radpro-monochrome PUBLIC DISPLAY_MONOCHROME)
target_include_directories(radpro-monochrome PRIVATE platform.io/lib/mcu-renderer platform.io/lib/mcu-max ${SERCOMM_INCLUDE_DIR})
target_link_libraries(radpro-monochrome
PRIVATE
$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
${SERCOMM_LIB}
)

add_executable(radpro-color-landscape ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-landscape PUBLIC DISPLAY_320X240)
target_compile_definitions(radpro-color-landscape PUBLIC DISPLAY_COLOR)
target_include_directories(radpro-color-landscape PRIVATE platform.io/lib/mcu-renderer platform.io/lib/mcu-max ${SERCOMM_INCLUDE_DIR})
target_link_libraries(radpro-color-landscape
PRIVATE
$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
${SERCOMM_LIB}
)

add_executable(radpro-color-portrait ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-portrait PUBLIC DISPLAY_240X320)
target_compile_definitions(radpro-color-portrait PUBLIC DISPLAY_COLOR)
target_include_directories(radpro-color-portrait PRIVATE platform.io/lib/mcu-renderer platform.io/lib/mcu-max ${SERCOMM_INCLUDE_DIR})
target_link_libraries(radpro-color-portrait
PRIVATE
$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
${SERCOMM_LIB}
)
