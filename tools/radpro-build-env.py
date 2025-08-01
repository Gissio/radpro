# Rad Pro
# Builds Rad Pro platform.io environments
#
# (C) 2022-2025 Gissio
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


def build_env(env_name, font_type, size, size_cgv, size_cjk):
    env = []

    # Get list of language header files
    language_files = [f for f in os.listdir(
        '../platform.io/src/strings') if f.endswith('.h')]

    for language_file in language_files:
        # Get language code by removing .h extension
        language = os.path.splitext(language_file)[0]

        # Set firmware size based on language
        if language in ['bg', 'el', 'ru', 'uk', 'vi']:
            firmware_size = size_cgv
        elif language in ['ja', 'ko', 'zh_CN']:
            firmware_size = size_cjk
        else:
            firmware_size = size

        # Write environment configuration
        env += [
            '\n']
        env += [
            f'[env:{env_name}-{language}]\n']
        env += [
            f'extends = {env_name}\n']
        env += [
            'build_flags =\n']
        env += [
            f'    ${{{env_name}.build_flags}}\n']
        env += [
            f'    -DFIRMWARE_SIZE={firmware_size}\n']
        env += [
            f'    -DLANGUAGE=\'"{language}"\'\n']
        env += [
            f'    -DSTRINGS=\'"strings/{language}.h"\'\n']
        env += [
            f'    -DFONT_SMALL=\'"fonts/font_{font_type}_small_{language}.h"\'\n']
        env += [
            f'    -DFONT_MEDIUM=\'"fonts/font_{font_type}_medium_{language}.h"\'\n']

    return env


# Build environments
env = read_file(get_radpro_path() + 'tools/platformio.ini.base')

env += build_env('fs2011-stm32f051c8', 'mono', '0xa800', '0xa800', '0xc000')
env += build_env('fs2011-gd32f150c8', 'mono', '0xa800', '0xa800', '0xc000')
env += build_env('fs2011-gd32f103c8', 'mono', '0xa800', '0xa800', '0xc000')
env += build_env('bosean-fs600', 'mono', '0xa800', '0xa800', '0xc000')
env += build_env('bosean-fs1000', 'mono', '0xa800', '0xa800', '0xc000')
env += build_env('bosean-fs5000', 'color', '0x10000', '0x10000', '0x1b000')
# env += build_env('bosean-fs5000_landscape', 'color', '0x10000', '0x10000', '0x1b000')
env += build_env('fnirsi-gc01_ch32f103r8', 'color_low', '0xa800', '0xb000', '0xb800')
env += build_env('fnirsi-gc01_apm32f103rb', 'color', '0x10000', '0x10000', '0x1b000')
env += build_env('gq-gmc800', 'color', '0x10000', '0x10000', '0x1b000')
# env += build_env('gq-gmc800_landscape', 'color', '0x10000', '0x10000', '0x1b000')

write_file(get_radpro_path() + 'platform.io/platformio.ini', env)
