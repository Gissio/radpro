# Rad Pro
# Pulse intervals capture
#
# (C) 2022-2023 Gissio
#
# License: MIT
#
# Notes:
# * To get the address for the 'gm' variable,
#   compile the firmware and run this command:
#
#   objdump -t platform.io/.pio/build/fs2011/firmware.elf | grep gm
#

import swd
import time

# Configuration
gm = 0x20000394

# Variables
dev = None

gmPulsesQueueHeadTail = gm
gmPulsesQueue = gm + 4
gmPulsesQueueSize = 16
gmPulsesQueueMask = gmPulsesQueueSize - 1

pulsesQueueTail = None
lastPulseTime = None

file = open('pulseintervals-data.bin', 'wb')

while(True):
    # Get data
    if dev == None:
        dev = swd.Swd()

    try:
        # Read data
        pulsesQueueHeadTail = dev.get_mem32(gmPulsesQueueHeadTail)
        pulsesQueue = [0] * gmPulsesQueueSize
        for i in range(0, gmPulsesQueueSize):
            pulsesQueue[i] = dev.get_mem32(gmPulsesQueue + 4 * i)

        # Process data
        pulsesQueueHead = pulsesQueueHeadTail & gmPulsesQueueMask
        if pulsesQueueTail == None:
            pulsesQueueTail = pulsesQueueHead

        lastPulsesQueueTail = pulsesQueueTail

        while pulsesQueueTail != pulsesQueueHead:
            pulseTime = pulsesQueue[pulsesQueueTail]

            if lastPulseTime != None:
                interval = (pulseTime - lastPulseTime) & 0xffffffff
                file.write(int.to_bytes(interval, 4))

            lastPulseTime = pulseTime
            pulsesQueueTail = (pulsesQueueTail + 1) & gmPulsesQueueMask

        if lastPulsesQueueTail != pulsesQueueHead:
            file.flush()

        # Wait
        time.sleep(0.1)

    except Exception as e:
        print()
        print(e)

        dev = None

        time.sleep(5)
