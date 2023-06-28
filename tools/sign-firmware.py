import array

# Calculate STM32 CRC
def getCRC(buffer, crc=0xffffffff):
    for i in range(0, len(buffer)):
        crc ^= buffer[(i & ~3) + (3 - i & 3)] << 24
        for _ in range(8):
            crc = crc << 1 if (crc & 0x80000000) == 0 else (crc << 1) ^ 0x104c11db7

    return crc

# Variables
input_path = 'platform.io/.pio/build/fs2011/firmware.bin'
output_dir = 'tools/'
version = '1.2.2'

FLASH_SIZE = 0x10000
FIRMWARE_SIZE = 0xc000 - 4

firmware = None

print('Reading input file...')

# Read unsigned firmware
with open(input_path, 'rb') as input_file:
    firmware = array.array('B', input_file.read())

    if len(firmware) > FIRMWARE_SIZE:
        print('Input file too large.')
        exit(1)

# Extend to flash size
firmware.extend([0xff] * (FLASH_SIZE - len(firmware)))

# Calculate CRC and sign
crc = getCRC(firmware[0:FIRMWARE_SIZE])
firmware[FIRMWARE_SIZE + 0] = (crc >> 0) & 0xff
firmware[FIRMWARE_SIZE + 1] = (crc >> 8) & 0xff
firmware[FIRMWARE_SIZE + 2] = (crc >> 16) & 0xff
firmware[FIRMWARE_SIZE + 3] = (crc >> 24) & 0xff

# Write install firmware
open(output_dir + 'radpro-fs2011-' + version + '-install.bin', 'wb').write(firmware)
print('Signed install firmware.')

# Write update firmware
open(output_dir + 'radpro-fs2011-' + version + '-update.bin', 'wb').write(firmware[0:(FIRMWARE_SIZE + 4)])
print('Signed update firmware.')
