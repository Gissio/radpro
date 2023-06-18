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

# Configuration
rng = 0x20000780

# Variables
dev = None

rngBitQueueHead = rng
rngBitQueueTail = rngBitQueueHead + 4
rngBitQueue = rngBitQueueTail + 4
rngBitQueueSize = 512
rngBitQueueMask = rngBitQueueSize - 1
rngBitQueueByteSize = rngBitQueueSize >> 3

rngValue = 0
rngValueBitIndex = 0

file = open('randombits-data.bin', 'wb')

while(True):
    # Get data
    if dev == None:
        dev = swd.Swd()

    try:
        bitQueueHead = dev.get_mem32(rngBitQueueHead)
        bitQueueTail = dev.get_mem32(rngBitQueueTail)
        bitQueue = dev.read_mem(rngBitQueue, rngBitQueueByteSize)

        # Process data
        bitQueueBits = []
        for byte in bitQueue:
            for i in range(0, 8):
                bitQueueBits.append((byte >> i) & 1)

        bitQueueTailLast = bitQueueTail

        while bitQueueTail != bitQueueHead:
            bit = bitQueueBits[bitQueueTail]
            rngValue |= bit << rngValueBitIndex

            rngValueBitIndex += 1
            if rngValueBitIndex >= 8:
                file.write(int.to_bytes(rngValue))

                rngValue = 0
                rngValueBitIndex = 0

            bitQueueTail = (bitQueueTail + 1) & rngBitQueueMask

        if bitQueueTailLast != bitQueueHead:
            dev.set_mem32(rngBitQueueTail, bitQueueTail)
            file.flush()

        # Wait
        time.sleep(0.1)

    except Exception as e:
        print(e)

        dev = None

        time.sleep(5)