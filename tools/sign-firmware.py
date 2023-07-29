import array
import os
import struct

version = '1.3.1beta1'

signature = 0x50444152 # "RADP"

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


def sign_firmware(input_path,
                  payload_size, flash_size, hardware_code, swd_data_address,
                  build_prefix):
    """ Sign firmware
    """
    global version

    footer_size = 0x10
    footer_index = payload_size - footer_size

    flash = bytearray(b'\xff' * flash_size)

    # Read compiled firmware
    if not os.path.exists(input_path):
        print("File not found: " + input_path)
        return

    print('Reading ' + input_path + '...')

    firmware = None
    with open(input_path, 'rb') as input_file:
        firmware = bytearray(input_file.read())

        if len(firmware) > footer_index:
            print('Input file too large.')
            return

    # Copy to flash
    flash[0:len(firmware)] = firmware

    # Calculate vresion data and CRC
    version_split = version.split('.')
    version_values = [
        int(version_split[0]),
        int(version_split[1]),
        ord(version_split[2][0]) - ord('0'),
    ]
    if len(version_split[2]) > 1:
        version_values.append(ord(version_split[2][-1]) - ord('0'))
    else:
        version_values.append(0)

    device_id = struct.unpack(">L", bytes([
        version_values[0] << 4 | version_values[1],
        version_values[2] << 4 | version_values[3],
        hardware_code,
        1 if flash_size == 0x10000 else 2,
    ]))[0]

    crc = get_crc(flash[0:footer_index])

    # Sign
    store_word(flash, footer_index + 0x0, signature)
    store_word(flash, footer_index + 0x4, device_id)
    store_word(flash, footer_index + 0x8, swd_data_address)
    store_word(flash, footer_index + 0xc, crc)

    # Write install firmware
    output_path = build_prefix + '-' + version + '-install.bin'
    open(output_path, 'wb').write(flash)
    print('Signed ' + output_path)

    # Write update firmware
    output_path = build_prefix + '-' + version + '-update.bin'
    open(output_path, 'wb').write(flash[0:payload_size])
    print('Signed ' + output_path)


# Variables
sign_firmware('../platform.io/.pio/build/fs2011-stm32f051c8/firmware.bin',
              0x8000, 0x10000, 0, 0x40002850,
              'radpro-fs2011-stm32f051')
sign_firmware('../platform.io/.pio/build/fs2011-gd32f150c8/firmware.bin',
              0x8000, 0x10000, 1, 0x40002850, 
              'radpro-fs2011-gd32f150')
sign_firmware('../platform.io/.pio/build/fs2011-gd32f103c8/firmware.bin',
              0x8000, 0x10000, 2, 0x40006c04,
              'radpro-fs2011-gd32f103')
sign_firmware('../platform.io/.pio/build/fs600-stm32g070cb/firmware.bin',
              0x8000, 0x20000, 3, 0x4000b100,
              'radpro-fs600-stm32g070')
