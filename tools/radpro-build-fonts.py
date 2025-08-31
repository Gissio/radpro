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
    args = []
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
    args += [get_radpro_path() + 'fonts/' + source]
    args += [get_radpro_path() + 'platform.io/src/fonts/' + dest]
    sys.argv[1:] = args
    fontconv.main()


def get_codepoint_set(text, codepoint_set=''):
    text = ''.join(text)

    codepoints = textproc.parse_codepoint_set(codepoint_set)

    text = textproc.filter_c_strings(text)

    codepoints.update({ord(char) for char in text})

    return ','.join(textproc.build_codepoint_set(codepoints))


# Codepoint sets
CODEPOINT_SET_SYMBOLS_MONOCHROME = '0x30-0x3e,0x41-0x46,0x49-0x4e,0x50-0x56'
CODEPOINT_SET_SYMBOLS_COLOR = '0x30-0x3e,0x41-0x46'
CODEPOINT_SET_SYMBOLS_COLOR_1BPP = '0x30-0x34,0x38-0x3e'
CODEPOINT_SET_LARGE = '0x2e,0x30-0x39,0x2012'

# Common fonts
run_fontconv('RadPro-Symbols8.bdf',
             CODEPOINT_SET_SYMBOLS_MONOCHROME,
             'font_symbols_monochrome.h',
             'font_symbols')
run_fontconv('RadPro-Symbols.ttf',
             CODEPOINT_SET_SYMBOLS_COLOR,
             'font_symbols_color.h',
             'font_symbols',
             pixels=24)
run_fontconv('RadPro-Symbols.ttf',
             CODEPOINT_SET_SYMBOLS_COLOR_1BPP,
             'font_symbols_color_1bpp.h',
             'font_symbols',
             pixels=24,
             bpp=1)

run_fontconv('RadPro-Sans33-Digits.bdf',
             CODEPOINT_SET_LARGE,
             'font_large_monochrome.h',
             'font_large')
run_fontconv('NotoSans-SemiBold.ttf',
             CODEPOINT_SET_LARGE,
             'font_large_color_115.h',
             'font_large',
             pixels=115,
             cap_height=82)
run_fontconv('NotoSans-SemiBold.ttf',
             CODEPOINT_SET_LARGE,
             'font_large_color_115_1bpp.h',
             'font_large',
             pixels=115,
             bpp=1,
             cap_height=82)
run_fontconv('NotoSans-SemiBold.ttf',
             CODEPOINT_SET_LARGE,
             'font_large_color_84.h',
             'font_large',
             pixels=84,
             cap_height=60)


# Languages
for language_file in Path('../platform.io/src/strings').glob('*.h'):
    language = language_file.stem

    # Get codepoint sets using textproc
    language_text = open(language_file, 'rt', encoding='utf-8').readlines()

    font_medium_matches = [
        'STRING_ELLIPSIS',
        'STRING_NANO',
        'STRING_MICRO',
        'STRING_MILLI',
        'STRING_KILO',
        'STRING_MEGA',
        'STRING_GIGA',
        'STRING_SV',
        'STRING_SVH',
        'STRING_REM',
        'STRING_REMH',
        'STRING_CPM',
        'STRING_CPS',
        'STRING_COUNT',
        'STRING_COUNTS',
    ]

    medium_text = [line for line in language_text
                   if any(match in line for match in font_medium_matches)]

    codepoint_set_small = get_codepoint_set(language_text, '0x20-0x7e')
    codepoint_set_monochrome_medium = get_codepoint_set(language_text)
    codepoint_set_color_medium = get_codepoint_set(medium_text)

    # Set font paths based on language
    if language == 'ja':
        font_small_monochrome = 'QuanPixel-7.bdf'
        font_medium_monochrome = 'FusionPixel-12.bdf'
        font_color = 'NotoSansJP-SemiBold.ttf'
    elif language == 'ko':
        font_small_monochrome = 'QuanPixel-7.bdf'
        font_medium_monochrome = 'FusionPixel-12.bdf'
        font_color = 'NotoSansKR-SemiBold.ttf'
    elif language == 'zh_CN':
        font_small_monochrome = 'QuanPixel-7.bdf'
        font_medium_monochrome = 'FusionPixel-12.bdf'
        font_color = 'NotoSansSC-SemiBold.ttf'
    else:
        font_small_monochrome = 'Tiny5-Regular.bdf'
        font_medium_monochrome = 'RadPro-Sans8.bdf'
        font_color = 'NotoSans-SemiBold.ttf'

    if language in ['ja', 'ko', 'zh_CN']:
        font_medium_color_32_ascent = 36
        font_medium_color_32_descent = 9
        font_medium_color_32_cap_height = 25
        font_small_color_21_ascent = 23
        font_small_color_21_descent = 7
        font_small_color_21_cap_height = 16
    else:
        font_medium_color_32_ascent = None
        font_medium_color_32_descent = None
        font_medium_color_32_cap_height = None
        font_small_color_21_ascent = None
        font_small_color_21_descent = None
        font_small_color_21_cap_height = None

    # Language-specific fonts
    run_fontconv(font_medium_monochrome,
                 codepoint_set_monochrome_medium,
                 f'font_medium_{language}_monochrome.h',
                 'font_medium')
    run_fontconv(font_color,
                 codepoint_set_color_medium,
                 f'font_medium_{language}_color_24.h',
                 'font_medium',
                 pixels=24)
    run_fontconv(font_color,
                 codepoint_set_color_medium,
                 f'font_medium_{language}_color_32.h',
                 'font_medium',
                 pixels=32,
                 ascent=font_medium_color_32_ascent,
                 descent=font_medium_color_32_descent,
                 cap_height=font_medium_color_32_cap_height)
    run_fontconv(font_color,
                 codepoint_set_color_medium,
                 f'font_medium_{language}_color_32_1bpp.h',
                 'font_medium',
                 pixels=32,
                 bpp=1,
                 ascent=font_medium_color_32_ascent,
                 descent=font_medium_color_32_descent,
                 cap_height=font_medium_color_32_cap_height)

    run_fontconv(font_small_monochrome,
                 codepoint_set_small,
                 f'font_small_{language}_monochrome.h',
                 'font_small')
    run_fontconv(font_color,
                 codepoint_set_small,
                 f'font_small_{language}_color_16.h',
                 'font_small',
                 pixels=16)
    run_fontconv(font_color,
                 codepoint_set_small,
                 f'font_small_{language}_color_21.h',
                 'font_small',
                 pixels=21,
                 ascent=font_small_color_21_ascent,
                 descent=font_small_color_21_descent,
                 cap_height=font_small_color_21_cap_height)
    run_fontconv(font_color,
                 codepoint_set_small,
                 f'font_small_{language}_color_21_1bpp.h',
                 'font_small',
                 pixels=21,
                 bpp=1,
                 ascent=font_small_color_21_ascent,
                 descent=font_small_color_21_descent,
                 cap_height=font_small_color_21_cap_height)
