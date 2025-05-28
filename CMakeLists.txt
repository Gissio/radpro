cmake_minimum_required(VERSION 3.16)

project(radpro)

set(CMAKE_C_STANDARD 99)

set(LANGUAGE "ru")

FILE(GLOB sources platform.io/src/*.c)
FILE(GLOB sdl_sources platform.io/src/sdl/*.c)
FILE(GLOB mcurenderer_sources platform.io/lib/mcu-renderer/*.c)
FILE(GLOB mcumax_sources platform.io/lib/mcu-max/*.c)

include_directories(platform.io/lib/mcu-renderer platform.io/lib/mcu-max ${SERCOMM_INCLUDE_DIR})

add_definitions(-DI18N="strings/${LANGUAGE}.h")
add_definitions(-DSIMULATOR)
add_definitions(-DSIMULATE_PULSES)
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_definitions(-DSIMULATE_COMM)
endif()
add_definitions(-DTUBE_HV_PWM)
# add_definitions(-DBATTERY_REMOVABLE)
# add_definitions(-DKEYBOARD_2KEYS)
# add_definitions(-DKEYBOARD_3KEYS)
add_definitions(-DKEYBOARD_4KEYS)
add_definitions(-DKEYBOARD_POWERON_NOWAIT)
add_definitions(-DVIBRATION)
add_definitions(-DPULSE_LED)
# add_definitions(-DALERT_LED)
# add_definitions(-DDATA_MODE)
add_definitions(-DGAME)
add_definitions(-DMCURENDERER_SDL)
# add_definitions(-DST7789_DEBUG)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
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
target_compile_definitions(radpro-monochrome PUBLIC
    DISPLAY_128X64
    DISPLAY_MONOCHROME
    FONT_SMALL="fonts/font_mono_small_${LANGUAGE}.h"
    FONT_MEDIUM="fonts/font_mono_medium_${LANGUAGE}.h"
    FONT_LARGE="fonts/font_mono_large.h"
    FONT_SYMBOLS="fonts/font_mono_symbols.h"
)

add_executable(radpro-color-landscape ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-landscape PUBLIC
    DISPLAY_320X240
    DISPLAY_COLOR
    FONT_SMALL="fonts/font_color_small_${LANGUAGE}.h"
    FONT_MEDIUM="fonts/font_color_medium_${LANGUAGE}.h"
    FONT_LARGE="fonts/font_color_large.h"
    FONT_SYMBOLS="fonts/font_color_nocharging_symbols.h"
)

add_executable(radpro-color-portrait ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-portrait PUBLIC
    DISPLAY_240X320
    DISPLAY_COLOR
    FONT_SMALL="fonts/font_color_small_${LANGUAGE}.h"
    FONT_MEDIUM="fonts/font_color_medium_${LANGUAGE}.h"
    FONT_LARGE="fonts/font_color_large_portrait.h"
    FONT_SYMBOLS="fonts/font_color_nocharging_symbols.h"
)
