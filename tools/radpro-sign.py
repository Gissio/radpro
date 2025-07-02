# Rad Pro
# Signs the Rad Pro firmware
#
# (C) 2022-2025 Gissio
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
    strings_dir = get_radpro_path() + 'platform.io/src/strings'
    strings_files = os.listdir(strings_dir)

    return [file[:-2] for file in strings_files if file.endswith('.h')]


def get_firmware_version():
    system_file = get_radpro_path() + 'platform.io/src/system.h'
    with open(system_file, 'r') as f:
        lines = f.readlines()

        for line in lines:
            if line.startswith('#define FIRMWARE_VERSION '):
                tokens = line.split('"')
                return tokens[1]


def extract_elf_data(elf_file):
    """ Get SWD comm
    """
    with open(elf_file, 'rb') as f:
        elf = ELFFile(f)

        symtab = elf.get_section_by_name('.symtab')
        comm = symtab.get_symbol_by_name('comm')[0].entry['st_value']

        comment_data = elf.get_section_by_name('.comment').data()
        comments = {}
        for line in comment_data.split(b'\x00')[:-1]:
            decoded_string = line.decode('ascii')
            pair = decoded_string.split(': ', 2)
            comments[pair[0]] = pair[1]

        flash_size = int(comments['FLASH_SIZE'], 16)
        firmware_offset = int(comments['FIRMWARE_BASE'], 16) - 0x08000000
        firmware_size = int(comments['FIRMWARE_SIZE'], 16)

        return comm, flash_size, firmware_offset, firmware_size

    return None


def get_crc(buffer, crc=0xffffffff):
    """ Calculate STM32 CRC
    """
    for i in range(0, len(buffer)):
        crc ^= buffer[(i & ~3) + (3 - i & 3)] << 24
        for _ in range(8):
            crc = crc << 1 if (crc & 0x80000000) == 0 else (
                crc << 1) ^ 0x104c11db7

    return crc


def read_word(buffer, address):
    buffer[address:address+4] = bytearray(struct.pack('<L', value))


def write_word(buffer, address, value):
    buffer[address:address+4] = bytearray(struct.pack('<L', value))


def sign_firmware(env_name):
    """ Sign firmware
    """

    env_manufacturer = env_name.split('-')[0]

    languages = get_languages()

    for language in languages:
        build_name = env_name + '-' + language
        build_dir = get_radpro_path() + 'platform.io/.pio/build/' + \
            build_name + '/'
        bin_file = build_dir + 'firmware.bin'
        elf_file = build_dir + 'firmware.elf'

        print('Building ' + build_name + '...')

        # Read compiled firmware
        if not os.path.exists(elf_file):
            print("warning: file not found: " + bin_file)
            continue

        firmware_version = get_firmware_version()
        swd_comm_address, flash_size, firmware_offset, firmware_size = extract_elf_data(elf_file)

        firmware = None
        with open(bin_file, 'rb') as f:
            firmware = bytearray(f.read())

        footer_size = 0x4
        footer_index = firmware_size - footer_size
        file_size = flash_size - firmware_offset

        flash = bytearray(b'\xff' * file_size)

        remaining_space = footer_index - len(firmware)

        if remaining_space < 0:
            print(
                f'warning: input file too large: remaining space {remaining_space}')

            continue

        # Sign
        flash[0:len(firmware)] = firmware

        write_word(flash, 0x20, 0x50444152)
        write_word(flash, 0x24, swd_comm_address)
        write_word(flash, footer_index, get_crc(flash[0:footer_index]))

        # Build firmware stem
        build_stem = 'radpro-' + env_name + '-' + language + '-' + firmware_version

        # Build install binary
        if firmware_offset != 0:
            install_path = env_manufacturer + '/' + 'install' + '/'
            output_path = install_path + build_stem + '-install.bin'

            os.makedirs(install_path, exist_ok=True)
            with open(output_path, 'wb') as f:
                f.write(flash)

        # Build firmware binary
        firmware_path = env_manufacturer + '/' + 'firmware' + '/'
        output_path = firmware_path + build_stem + '.bin'

        os.makedirs(firmware_path, exist_ok=True)
        with open(output_path, 'wb') as f:
            f.write(flash[0:firmware_size])

        print(
            f'done: remaining space {remaining_space}')


# Sign firmware
sign_firmware('fs2011-stm32f051c8')
sign_firmware('fs2011-gd32f150c8')
sign_firmware('fs2011-gd32f103c8')
sign_firmware('bosean-fs600')
sign_firmware('bosean-fs1000')
sign_firmware('bosean-fs5000_portrait')
sign_firmware('bosean-fs5000_landscape')
sign_firmware('fnirsi-gc01_ch32f103r8')
sign_firmware('fnirsi-gc01_apm32f103rb')
sign_firmware('gq-gmc800_portrait')
sign_firmware('gq-gmc800_landscape')
