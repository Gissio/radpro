# Rad Pro
# Builds Rad Pro platform.io environments
#
# (C) 2022-2026 Gissio
#
# License: MIT
#

import os


def get_radpro_path():
    tools_path, _ = os.path.split(__file__)

    return tools_path + '/../'


def read_file(path):
    with open(path, 'rt') as file:
        return file.readlines()


def write_file(path, env):
    with open(path, 'wt') as file:
        return file.writelines(env)


# size_cgv: cyrillic, greek, vietnamese
# size_cjk: chinese, japanese, korean
def build_env(env_name,
              font_small, font_medium, font_large, font_symbols,
              font_low,
              firmware_size_regular, firmware_size_cgv, firmware_size_cjk):
    env = []

    # Get list of language header files
    language_files = [f for f in os.listdir(
        '../platform.io/src/system/strings') if f.endswith('.h')]

    for language_file in language_files:
        # Get language code by removing .h extension
        language = os.path.splitext(language_file)[0]

        # Set firmware size based on language
        if language in ['bg', 'el', 'ru', 'uk', 'vi']:
            firmware_size = firmware_size_cgv
        elif language in ['ja', 'ko', 'zh_CN']:
            firmware_size = firmware_size_cjk
        else:
            firmware_size = firmware_size_regular

        if font_low:
            if language in ['en']:
                font_small_bpp = font_small + '_2bpp'
                font_medium_bpp = font_medium + '_2bpp'
                font_large_bpp = font_large + '_2bpp'
                font_symbols_bpp = font_symbols + '_2bpp'
            else:
                font_small_bpp = font_small + '_1bpp'
                font_medium_bpp = font_medium + '_1bpp'
                font_large_bpp = font_large + '_1bpp'
                font_symbols_bpp = font_symbols + '_1bpp'
        else:
            font_small_bpp = font_small
            font_medium_bpp = font_medium
            font_large_bpp = font_large
            font_symbols_bpp = font_symbols

        # Write environment configuration
        env += ['\n']
        env += [f'[env:{env_name}-{language}]\n']
        env += [f'extends = {env_name}\n']
        env += ['build_flags =\n']
        env += [f'    ${{{env_name}.build_flags}}\n']
        env += [f'    -DFIRMWARE_SIZE={firmware_size}\n']
        env += [f'    -DLANGUAGE=\'"{language}"\'\n']
        env += [f'    -DSTRINGS=\'"strings/{language}.h"\'\n']
        env += [f'    -DFONT_SMALL=\'"fonts/font_small_{language}_{font_small_bpp}.h"\'\n']
        env += [f'    -DFONT_MEDIUM=\'"fonts/font_medium_{language}_{font_medium_bpp}.h"\'\n']
        env += [f'    -DFONT_LARGE=\'"fonts/font_large_{font_large_bpp}.h"\'\n']
        env += [f'    -DFONT_SYMBOLS=\'"fonts/font_symbols_{font_symbols_bpp}.h"\'\n']

    return env


# Build environments
env = read_file(get_radpro_path() + 'tools/platformio.ini.base')

env += build_env('fs2011-stm32f051c8', 'monochrome', 'monochrome', 'monochrome', 'monochrome', False,  '0xa800', '0xb000', '0xc800')
env += build_env('fs2011-gd32f150c8', 'monochrome', 'monochrome', 'monochrome', 'monochrome', False, '0xa800', '0xb000', '0xc800')
env += build_env('fs2011-gd32f103c8', 'monochrome', 'monochrome', 'monochrome', 'monochrome', False, '0xa800', '0xb000', '0xc800')
env += build_env('bosean-fs600', 'monochrome', 'monochrome', 'monochrome', 'monochrome', False, '0xa800', '0xb000', '0xc800')
env += build_env('bosean-fs1000', 'monochrome', 'monochrome', 'monochrome', 'monochrome', False, '0xa800', '0xb000', '0xc800')
env += build_env('bosean-fs5000', 'color_21', 'color_32', 'color_84', 'color', False, '0xd000', '0xe000', '0x14000')
# env += build_env('bosean-fs5000_landscape', 'color_21','color_32', 'color_115', 'color', False, '0x10000', '0x10000', '0x1b000')
env += build_env('fnirsi-gc01_ch32f103r8', 'color_21', 'color_32', 'color_115', 'color', True, '0xa800', '0xb000', '0xb800')
env += build_env('fnirsi-gc01_apm32f103rb', 'color_21', 'color_32', 'color_115', 'color', False, '0xe000', '0xf000', '0x15000')
env += build_env('fnirsi-gc03', 'color_21', 'color_32', 'color_84', 'color', False, '0xe000', '0x10000', '0x16000')
env += build_env('gq-gmc800', 'color_16', 'color_24', 'color_84', 'color', False, '0xd000', '0xe000', '0x13000')
# env += build_env('gq-gmc800_landscape', 'color_12', 'color_16', 'color_115', 'color', False, '0x10000', '0xd800', '0x14000')

write_file(get_radpro_path() + 'platform.io/platformio.ini', env)
