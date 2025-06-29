# Rad Pro
# Builds Rad Pro fonts
#
# (C) 2022-2025 Gissio
#
# License: MIT
#

import os
from pathlib import Path
import sys

import fontconv
import textproc


def get_radpro_path():
    tools_path, _ = os.path.split(__file__)

    return tools_path + '/../'


def run_fontconv(source,
                 codepoint_set,
                 dest,
                 name,
                 pixels=None,
                 bpp=None,
                 ascent=None,
                 descent=None,
                 cap_height=None):
    args=[]
    args += ['-s', codepoint_set]
    args += ['-n', name]
    if pixels:
        args += ['-p', str(pixels)]
    if bpp:
        args += ['-b', str(bpp)]
    if ascent:
        args += ['-a', str(ascent)]
    if descent:
        args += ['-d', str(descent)]
    if cap_height:
        args += ['-c', str(cap_height)]
    args += [get_radpro_path() + "fonts/" + source]
    args += [get_radpro_path() + "platform.io/src/fonts/" + dest]
    sys.argv[1:] = args
    fontconv.main()


def get_codepoint_set(filename, codepoint_set=""):
    codepoints = textproc.parse_codepoint_set(codepoint_set)

    text = open(filename, 'rt', encoding='utf-8').read()
    text = textproc.filter_c_strings(text)

    codepoints.update({ord(char) for char in text})

    return ','.join(textproc.build_codepoint_set(codepoints))


# Codepoint sets
CODEPOINT_SET_LARGE = "0x2e,0x30-0x39,0x2012"
CODEPOINT_SET_MONO_SYMBOLS = "0x30-0x3e,0x41-0x46,0x49-0x4e,0x50-0x56"
CODEPOINT_SET_COLOR_SYMBOLS = "0x30-0x3e,0x41-0x46"
CODEPOINT_SET_COLOR_LOW_SYMBOLS = "0x30-0x34,0x3a-0x3e"


# Common font conversions
run_fontconv("RadPro-Sans33-Digits.bdf",
             CODEPOINT_SET_LARGE,
             "font_mono_large.h",
             "font_large")
run_fontconv("OpenSans-SemiBoldFigureDash.ttf",
             CODEPOINT_SET_LARGE,
             "font_color_large.h",
             "font_large",
             pixels=118,
             ascent=113,
             cap_height=86)
run_fontconv("OpenSans-SemiBoldFigureDash.ttf",
             CODEPOINT_SET_LARGE,
             "font_color_large_portrait.h",
             "font_large",
             pixels=86,
             ascent=83,
             cap_height=63)
run_fontconv("OpenSans-SemiBoldFigureDash.ttf",
             CODEPOINT_SET_LARGE,
             "font_color_low_large.h",
             "font_large",
             pixels=118,
             bpp=2,
             ascent=113,
             cap_height=86)
run_fontconv("OpenSans-SemiBoldFigureDash.ttf",
             CODEPOINT_SET_LARGE,
             "font_color_verylow_large.h",
             "font_large",
             pixels=118,
             bpp=1,
             ascent=113,
             cap_height=86)

run_fontconv("RadPro-Symbols8.bdf",
             CODEPOINT_SET_MONO_SYMBOLS,
             "font_mono_symbols.h",
             "font_symbols")
run_fontconv("RadPro-Symbols.ttf",
             CODEPOINT_SET_COLOR_SYMBOLS,
             "font_color_symbols.h",
             "font_symbols",
             pixels=24)
run_fontconv("RadPro-Symbols.ttf",
             CODEPOINT_SET_COLOR_LOW_SYMBOLS,
             "font_color_low_symbols.h",
             "font_symbols",
             pixels=24,
             bpp=2)
run_fontconv("RadPro-Symbols.ttf",
             CODEPOINT_SET_COLOR_LOW_SYMBOLS,
             "font_color_verylow_symbols.h",
             "font_symbols",
             pixels=24,
             bpp=1)

# Languages
for language_file in Path("../platform.io/src/strings").glob("*.h"):
    language = language_file.stem

    # Get codepoint sets using textproc
    textproc.build_codepoint_set
    codepoint_set_small = get_codepoint_set(language_file, "0x20-0x7e")
    codepoint_set_medium = get_codepoint_set(language_file)

    # Set font paths based on language
    if language == "ja":
        font_mono_small = "QuanPixel-7.bdf"
        font_mono_medium = "FusionPixel-12.bdf"
        font_color = "NotoSansJP-Medium.ttf"
    elif language == "ko":
        font_mono_small = "QuanPixel-7.bdf"
        font_mono_medium = "FusionPixel-12.bdf"
        font_color = "NotoSansKR-Medium.ttf"
    elif language == "zh_CN":
        font_mono_small = "QuanPixel-7.bdf"
        font_mono_medium = "FusionPixel-12.bdf"
        font_color = "NotoSansSC-Medium.ttf"
    else:
        font_mono_small = "Tiny5-Regular.bdf"
        font_mono_medium = "RadPro-Sans8.bdf"
        font_color = "OpenSans-Medium.ttf"

    # Set bpp based on language
    if language == "en":
        bpp_low = 2
    else:
        bpp_low = 1

    # Language-specific font conversions
    run_fontconv(font_mono_small,
                 codepoint_set_small,
                 f"font_mono_small_{language}.h",
                 "font_small",
                 ascent=7,
                 descent=1,
                 cap_height=5)
    run_fontconv(font_color,
                 codepoint_set_small,
                 f"font_color_small_{language}.h",
                 "font_small",
                 pixels=16.8,
                 ascent=18,
                 descent=6,
                 cap_height=12)
    run_fontconv(font_color,
                 codepoint_set_small,
                 f"font_color_low_small_{language}.h",
                 "font_small",
                 pixels=16.8,
                 bpp=bpp_low,
                 ascent=18,
                 descent=6,
                 cap_height=12)

    run_fontconv(font_mono_medium,
                 codepoint_set_medium,
                 f"font_mono_medium_{language}.h",
                 "font_medium",
                 ascent=10,
                 descent=2,
                 cap_height=8)
    run_fontconv(font_color,
                 codepoint_set_medium,
                 f"font_color_medium_{language}.h",
                 "font_medium",
                 pixels=25.2,
                 ascent=24,
                 descent=8,
                 cap_height=18)
    run_fontconv(font_color,
                 codepoint_set_medium,
                 f"font_color_low_medium_{language}.h",
                 "font_medium",
                 pixels=25.2,
                 bpp=bpp_low,
                 ascent=24,
                 descent=8,
                 cap_height=18)
