# Rad Pro
# Signs the Rad Pro firmware
#
# (C) 2022-2026 Gissio
#
# License: MIT
#

import os
import struct

from elftools.elf.elffile import ELFFile


def get_radpro_path():
    tools_path, _ = os.path.split(__file__)

    return tools_path + '/../'


def get_languages():
    strings_dir = get_radpro_path() + 'platform.io/src/system/strings'
    strings_files = os.listdir(strings_dir)

    return [file[:-2] for file in strings_files if file.endswith('.h')]


def get_firmware_version():
    system_file = get_radpro_path() + 'platform.io/src/system/system.h'
    with open(system_file, 'r') as f:
        lines = f.readlines()

        for line in lines:
            if line.startswith('#define FIRMWARE_VERSION '):
                tokens = line.split('"')
                return tokens[1]


def extract_elf_data(elf_file):
    with open(elf_file, 'rb') as f:
        elf = ELFFile(f)

        comment_data = elf.get_section_by_name('.comment').data()
        comments = {}
        for line in comment_data.split(b'\x00')[:-1]:
            decoded_string = line.decode('ascii')
            pair = decoded_string.split(': ', 2)
            comments[pair[0]] = pair[1]

        flash_base = eval(comments['FLASH_BASE'])
        flash_size = eval(comments['FLASH_SIZE'])
        firmware_base = eval(comments['FIRMWARE_BASE'])
        firmware_size = eval(comments['FIRMWARE_SIZE'])
        states_base = eval(comments['STATES_BASE'])
        states_size = eval(comments['STATES_SIZE'])
        datalog_base = eval(comments['DATALOG_BASE'])
        datalog_size = eval(comments['DATALOG_SIZE'])

        firmware_offset = firmware_base - flash_base
        image_size = (datalog_base - firmware_base) + datalog_size

        return firmware_offset, firmware_size, image_size, states_base, states_size

    return None


def get_crc(buffer, crc=0xffffffff):
    ''' Calculate STM32 CRC
    '''
    for i in range(0, len(buffer)):
        crc ^= buffer[(i & ~3) + (3 - i & 3)] << 24
        for _ in range(8):
            crc = crc << 1 if (crc & 0x80000000) == 0 else (
                crc << 1) ^ 0x104c11db7

    return crc


def write_word(buffer, address, value):
    buffer[address:address+4] = bytearray(struct.pack('<L', value))


def sign_firmware(env_name):
    ''' Sign firmware
    '''

    languages = get_languages()

    for language in languages:
        build_name = env_name + '-' + language
        build_dir = get_radpro_path() + 'platform.io/.pio/build/' + \
            build_name + '/'
        bin_file = build_dir + 'firmware.bin'
        elf_file = build_dir + 'firmware.elf'

        print('Building ' + build_name + '...')

        # Read firmware
        if not os.path.exists(elf_file):
            print('warning: file not found: ' + bin_file)
            continue

        firmware_version = get_firmware_version()
        firmware_offset, firmware_size, image_size, states_base, states_size = extract_elf_data(
            elf_file)

        firmware = None
        with open(bin_file, 'rb') as f:
            firmware = bytearray(f.read())

        firmware_footer_size = 0x4
        firmware_footer_offset = firmware_size - firmware_footer_size

        image = bytearray(b'\xff' * image_size)

        firmware_empty_space = firmware_footer_offset - len(firmware)

        if firmware_empty_space < 0:
            print(
                f'warning: input file too large: size 0x{firmware_size:04x}, empty space {firmware_empty_space}')

            continue

        # Sign
        image[0:len(firmware)] = firmware

        write_word(image, 0x20, states_base)
        write_word(image, 0x24, states_size)
        write_word(image, firmware_footer_offset,
                   get_crc(image[0:firmware_footer_offset]))

        # Build firmware stem
        build_stem = 'radpro-' + env_name + '-' + language + '-' + firmware_version

        # Build install binary
        # if firmware_offset != 0:
        #     install_path = env_name + '/' + 'install' + '/'
        #     output_path = install_path + build_stem + '-install.bin'

        #     os.makedirs(install_path, exist_ok=True)
        #     with open(output_path, 'wb') as f:
        #         f.write(image)

        # Build firmware binary
        firmware_path = env_name + '/' + 'firmware' + '/'
        output_path = firmware_path + build_stem + '.bin'

        os.makedirs(firmware_path, exist_ok=True)
        with open(output_path, 'wb') as f:
            f.write(image[0:firmware_size])

        print(f'done: size 0x{firmware_size:04x}, empty space {firmware_empty_space}')


# Sign firmware
envs = [
    'fs2011-stm32f051c8',
    'fs2011-gd32f150c8',
    'fs2011-gd32f103c8',
    'bosean-fs600',
    'bosean-fs1000',
    'bosean-fs5000',
    # 'bosean-fs5000_landscape',
    'fnirsi-gc01_ch32f103r8',
    'fnirsi-gc01_apm32f103rb',
    'fnirsi-gc03',
    'gq-gmc800',
    # 'gq-gmc800_landscape',
]

for env in envs:
    sign_firmware(env)
