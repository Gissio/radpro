# FS2011 Pro
# Data Logger
#
# (C) 2022-2023 Gissio
#
# License: MIT
#
# Notes:
# * Te determine the addresses for the 'gm', 'rng' and 'averageRate' variables, run:
#
#   objdump -t platform.io/.pio/fs2011/firmware.elf | grep gm
#   objdump -t platform.io/.pio/fs2011/firmware.elf | grep rng
#   objdump -t platform.io/.pio/fs2011/firmware.elf | grep averageRate
#

from datetime import datetime
import swd
import time

dev = None
path = 'logs/'

# Addresses
gm = 0x20000384
gmPulsesQueueHeadTail = gm
gmPulsesQueue = gm + 4
gmPulsesQueueSize = 16
gmPulsesQueueMask = gmPulsesQueueSize - 1

rng = 0x20000780
rngBitQueueHead = rng
rngBitQueueTail = rngBitQueueHead + 4
rngBitQueue = rngBitQueueTail + 4
rngBitQueueSize = 512
rngBitQueueMask = rngBitQueueSize - 1
rngBitQueueByteSize = rngBitQueueSize >> 3

averageRate = 0x200003d8
averageRateSnapshotTime = averageRate + 4 * 4
averageRateSnapshotCount = averageRate + 5 * 4

# File management
fileIntervals = None
fileRNG = None
fileLog = None
fileDateLast = None

# Variables
pulsesQueueTail = None
lastPulseTime = None

rngValue = 0
rngValueBitIndex = 0

snapshotLastTime = None
snapshotLastCount = None

while(True):
    # Get data
    if dev == None:
        dev = swd.Swd()

    try:
        pulsesQueueHeadTail = dev.get_mem32(gmPulsesQueueHeadTail)
        pulsesQueue = [0] * gmPulsesQueueSize
        for i in range(0, gmPulsesQueueSize):
            pulsesQueue[i] = dev.get_mem32(gmPulsesQueue + 4 * i)

        bitQueueHead = dev.get_mem32(rngBitQueueHead)
        bitQueueTail = dev.get_mem32(rngBitQueueTail)
        bitQueue = dev.read_mem(rngBitQueue, rngBitQueueByteSize)

        snapshotTime = dev.get_mem32(averageRateSnapshotTime)
        snapshotCount = dev.get_mem32(averageRateSnapshotCount)

        now = datetime.now()

        # File management
        fileDate = now.strftime("%Y-%m-%d")
        if fileDate != fileDateLast:
            fileDateLast = fileDate

            fileIntervals = open(path + fileDate + '_intervals.bin', 'ab') 
            fileRNG = open(path + fileDate + '_randombits.bin', 'ab')
            fileLog = open(path + fileDate + '_samples.csv', 'a')

            if fileLog.tell() == 0:
                fileLog.write('"datetime", "counts"\n')

        # Process intervals
        pulsesQueueHead = pulsesQueueHeadTail & gmPulsesQueueMask
        if pulsesQueueTail == None:
            pulsesQueueTail = pulsesQueueHead

        while pulsesQueueTail != pulsesQueueHead:
            print('.', end='', flush=True)

            pulseTime = pulsesQueue[pulsesQueueTail]

            if lastPulseTime != None:
                interval = (pulseTime - lastPulseTime) & 0xffffffff

                fileIntervals.write(int.to_bytes(interval, 4))
                fileIntervals.flush()

            lastPulseTime = pulseTime
            pulsesQueueTail = (pulsesQueueTail + 1) & gmPulsesQueueMask

        # Process randombits
        bitQueueBits = []
        for byte in bitQueue:
            for i in range(0, 8):
                bitQueueBits.append((byte >> i) & 1)

        bitQueueTailLast = bitQueueTail

        while bitQueueTail != bitQueueHead:
            # print('.', end='', flush=True)
            bit = bitQueueBits[bitQueueTail]
            rngValue |= bit << rngValueBitIndex

            rngValueBitIndex += 1
            if rngValueBitIndex >= 8:
                fileRNG.write(int.to_bytes(rngValue))

                rngValue = 0
                rngValueBitIndex = 0

            bitQueueTail = (bitQueueTail + 1) & rngBitQueueMask

        if bitQueueTailLast != bitQueueHead:
            dev.set_mem32(rngBitQueueTail, bitQueueTail)
            fileRNG.flush()

        # Process samples
        if snapshotLastTime != snapshotTime:
            if snapshotLastCount == None:
                snapshotLastCount = snapshotCount 

            deltaCount = (snapshotCount - snapshotLastCount) & 0xffffffff
            snapshotDate = now.strftime('%Y-%m-%dT%H:%M:%S.%f')
            fileLog.write('"' + snapshotDate + '", ' + str(deltaCount) + '\n')
            fileLog.flush()

            snapshotLastTime = snapshotTime
            snapshotLastCount = snapshotCount

        # Wait
        time.sleep(0.1)

    except:
        dev = None

        pulsesQueueTail = None
        lastPulseTime = None

        rngValue = 0
        rngValueBitIndex = 0

        snapshotLastTime = None
        snapshotLastCount = None

        time.sleep(5)
