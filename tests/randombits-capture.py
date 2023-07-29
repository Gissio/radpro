# Rad Pro
# Random bits capture
#
# (C) 2022-2023 Gissio
#
# License: MIT
#
# Notes:
# * To get the address for the 'rng' variable,
#   compile the firmware and run this command:
#
#   objdump -t platform.io/.pio/build/fs2011/firmware.elf | grep rng
#

import swd
import time

# Rad Pro variables
rng = 0x20000000

# Local variables
rng_bit_queue_head = rng
rng_bit_queue_tail = rng_bit_queue_head + 4
rng_bit_queue = rng_bit_queue_tail + 4
rng_bit_queue_size = 512
rng_bit_queue_mask = rng_bit_queue_size - 1
rng_bit_queue_byte_size = rng_bit_queue_size >> 3

dev = None

rng_value = 0
rng_value_bit_index = 0

bit_queue_head = None
bit_queue_tail = None
bit_queue = None

file = open('randombits-data.bin', 'wb')

while (True):
    # Get data
    try:
        if dev == None:
            dev = swd.Swd()

        bit_queue_head = dev.get_mem32(rng_bit_queue_head)
        bit_queue_tail = dev.get_mem32(rng_bit_queue_tail)
        bit_queue = dev.read_mem(rng_bit_queue, rng_bit_queue_byte_size)

    except Exception as e:
        print()
        print('Exception ' + str(e.__class__) + ' ' + str(e))

        dev = None

        time.sleep(5)

    # Process data
    bit_queue_bits = []
    for byte in bit_queue:
        for i in range(0, 8):
            bit_queue_bits.append((byte >> i) & 1)

    bit_queue_tail_last = bit_queue_tail

    while bit_queue_tail != bit_queue_head:
        bit = bit_queue_bits[bit_queue_tail]
        rng_value |= bit << rng_value_bit_index

        rng_value_bit_index += 1
        if rng_value_bit_index >= 8:
            file.write(int.to_bytes(rng_value))

            rng_value = 0
            rng_value_bit_index = 0

        bit_queue_tail = (bit_queue_tail + 1) & rng_bit_queue_mask

    if bit_queue_tail_last != bit_queue_head:
        dev.set_mem32(rng_bit_queue_tail, bit_queue_tail)
        file.flush()

    # Wait
    time.sleep(0.1)
