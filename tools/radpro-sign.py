# Rad Pro
# Signs the Rad Pro firmware
#
# (C) 2022-2023 Gissio
#
# License: MIT
#

import os
import struct

from elftools.elf.elffile import ELFFile


def get_version(system_h_file):
    with open(system_h_file, 'r') as f:
        lines = f.readlines()

        for line in lines:
            if line.startswith('#define FIRMWARE_VERSION '):
                tokens = line.split('"')
                return tokens[1]


def get_swd_comm_address(elf_file):
    """ Get SWD comm
    """
    with open(elf_file, 'rb') as f:
        elf = ELFFile(f)

        symtab = elf.get_section_by_name('.symtab')
        comm = symtab.get_symbol_by_name('comm')[0].entry['st_value']

    return comm


def get_crc(buffer, crc=0xffffffff):
    """ Calculate STM32 CRC
    """
    for i in range(0, len(buffer)):
        crc ^= buffer[(i & ~3) + (3 - i & 3)] << 24
        for _ in range(8):
            crc = crc << 1 if (crc & 0x80000000) == 0 else (
                crc << 1) ^ 0x104c11db7

    return crc


def store_word(buffer, address, value):
    buffer[address:address+4] = bytearray(struct.pack('<L', value))


def sign_firmware(env_name, container_offset, container_size, flash_size):
    """ Sign firmware
    """

    tools_path, _ = os.path.split(__file__)
    system_h_file = tools_path + '/../platform.io/src/system.h'
    bin_file = tools_path + '/../platform.io/.pio/build/' + env_name + '/' + 'firmware.bin'
    elf_file = tools_path + '/../platform.io/.pio/build/' + env_name + '/' + 'firmware.elf'

    # Read compiled firmware
    if not os.path.exists(elf_file):
        print("File not found: " + bin_file)
        return

    print('Reading ' + bin_file + '...')

    version = get_version(system_h_file)
    swd_comm_address = get_swd_comm_address(elf_file)

    footer_size = 0x4
    footer_index = container_size - footer_size
    file_size = flash_size - container_offset

    flash = bytearray(b'\xff' * file_size)

    firmware = None
    with open(bin_file, 'rb') as f:
        firmware = bytearray(f.read())

        if len(firmware) > footer_index:
            print('Input file too large.')
            return

    # Sign
    flash[0:len(firmware)] = firmware

    store_word(flash, 0x20, 0x50444152)
    store_word(flash, 0x24, swd_comm_address)
    store_word(flash, footer_index, get_crc(flash[0:footer_index]))

    # Write signed binary
    build_prefix = 'radpro-' + env_name + '-' + version

    if container_offset != 0:
        output_path = build_prefix + '-install.bin'
        with open(output_path, 'wb') as f:
            f.write(flash)
            print('Signed ' + output_path)

    output_path = build_prefix + '.bin'
    with open(output_path, 'wb') as f:
        f.write(flash[0:container_size])
        print('Signed ' + output_path)


# Variables
sign_firmware('fs2011-stm32f051c8', 0x0, 0x9000, 0x10000)
sign_firmware('fs2011-gd32f150c8', 0x0, 0x9000, 0x10000)
sign_firmware('fs2011-gd32f103c8', 0x0, 0x9000, 0x10000,)
sign_firmware('bosean-fs600', 0x0, 0x9000, 0x20000)
sign_firmware('bosean-fs1000', 0x0, 0x9000, 0x20000)
sign_firmware('bosean-fs5000', 0x0, 0xc000, 0x40000)
sign_firmware('fnirsi-gc01-ch32f103r8', 0x4000, 0xa000, 0x10000)
sign_firmware('fnirsi-gc01-apm32f103rb', 0x4000, 0xc800, 0x20000)
