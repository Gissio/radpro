#!/bin/sh
set -eu

cd "$(dirname "$0")/.."
build_dir=$(mktemp -d "${TMPDIR:-/tmp}/radpro-raw.XXXXXX")
trap 'rm -f "$build_dir/raw-counter"; rmdir "$build_dir"' EXIT

src=platform.io/src
renderer=platform.io/lib/mcu-renderer
"${CC:-cc}" -std=c11 -O2 -ffunction-sections -fdata-sections -Wl,--gc-sections \
    -DH240 -DRADPRO_RAW_COUNTER -DSTART_POWERED -DDATA_MODE \
    -DKEYBOARD_5_KEYS -DDISPLAY_COLOR -DDISPLAY_320X240 '-DSTRINGS="strings/fr.h"' \
    '-DFONT_LARGE="ui/fonts/font_large_color_84.h"' \
    -I"$src" -I"$renderer" \
    tests/raw-counter.c "$src/main.c" "$renderer/mcu-renderer.c" \
    "$src/measurements/average.c" "$src/measurements/cumulative.c" \
    "$src/measurements/datalog.c" "$src/measurements/instantaneous.c" \
    "$src/measurements/measurements.c" "$src/measurements/pulses.c" \
    "$src/system/cmath.c" "$src/system/cstring.c" "$src/system/events.c" \
    "$src/system/power.c" "$src/system/settings.c" \
    -lm -o "$build_dir/raw-counter"
"$build_dir/raw-counter"
