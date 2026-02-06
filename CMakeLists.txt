cmake_minimum_required(VERSION 3.16)

project(radpro)

set(CMAKE_C_STANDARD 11)

set(LANGUAGE "en")

FILE(GLOB_RECURSE sources platform.io/src/*.c)
FILE(GLOB sdl_sources platform.io/src/sdl/*.c)
FILE(GLOB mcurenderer_sources platform.io/lib/mcu-renderer/*.c)
FILE(GLOB mcumax_sources platform.io/lib/mcu-max/*.c)

include_directories(platform.io/lib/mcu-renderer platform.io/lib/mcu-max ${SERCOMM_INCLUDE_DIR})

add_definitions(-DLANGUAGE="${LANGUAGE}")
add_definitions(-DSTRINGS="strings/${LANGUAGE}.h")
add_definitions(-DSIMULATOR)

add_definitions(-DSIMULATOR)
add_definitions(-DFLASH_BASE_=0x0)
add_definitions(-DFLASH_SIZE_=0x1000)
add_definitions(-DFLASH_PAGE_SIZE=0x400)
add_definitions(-DFLASH_WORD_SIZE=0x2)
add_definitions(-DFIRMWARE_BASE=0)
add_definitions(-DFIRMWARE_SIZE=0)
# add_definitions(-DST7789_DEBUG)
add_definitions(-DSTART_POWERED)
# add_definitions(-DBATTERY_REMOVABLE)
add_definitions(-DTUBE_HV_PWM)
add_definitions(-DSIMULATE_PULSES)
# add_definitions(-DEMFMETER)
# add_definitions(-DDATA_MODE)
add_definitions(-DKEYBOARD_5_KEYS)
add_definitions(-DKEYBOARD_KEY_POWER_OK)
add_definitions(-DMCURENDERER_SDL)
add_definitions(-DBUZZER)
# add_definitions(-DVOICE)
# add_definitions(-DPULSESOUND_ENABLE)
add_definitions(-DVIBRATOR)
# add_definitions(-DPULSE_LED)
# add_definitions(-DALERT_LED)
add_definitions(-DPULSE_LED_EN)
add_definitions(-DALERT_LED_EN)
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
add_definitions(-DSIMULATE_COMM)
endif()
add_definitions(-DGAME)

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
    FONT_SYMBOLS="fonts/font_symbols_monochrome.h"
    FONT_LARGE="fonts/font_large_monochrome.h"
    FONT_SMALL="fonts/font_small_${LANGUAGE}_monochrome.h"
    FONT_MEDIUM="fonts/font_medium_${LANGUAGE}_monochrome.h"
)

add_executable(radpro-color-landscape ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-landscape PUBLIC
    DISPLAY_320X240
    DISPLAY_COLOR
    FONT_SYMBOLS="fonts/font_symbols_color.h"
    FONT_LARGE="fonts/font_large_color_115.h"
    FONT_SMALL="fonts/font_small_${LANGUAGE}_color_21.h"
    FONT_MEDIUM="fonts/font_medium_${LANGUAGE}_color_32.h"
)

add_executable(radpro-color-portrait ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-portrait PUBLIC
    DISPLAY_240X320
    DISPLAY_COLOR
    FONT_SYMBOLS="fonts/font_symbols_color.h"
    FONT_LARGE="fonts/font_large_color_84.h"
    FONT_SMALL="fonts/font_small_${LANGUAGE}_color_21.h"
    FONT_MEDIUM="fonts/font_medium_${LANGUAGE}_color_32.h"
)

add_executable(radpro-color-portrait-125ppi ${sources} ${display_sources} ${sdl_sources} ${mcurenderer_sources} ${mcumax_sources})
target_compile_definitions(radpro-color-portrait-125ppi PUBLIC
    DISPLAY_240X320
    DISPLAY_125PPI
    DISPLAY_COLOR
    FONT_SYMBOLS="fonts/font_symbols_color.h"
    FONT_LARGE="fonts/font_large_color_84.h"
    FONT_SMALL="fonts/font_small_${LANGUAGE}_color_16.h"
    FONT_MEDIUM="fonts/font_medium_${LANGUAGE}_color_24.h"
)
