# Rad Pro
# Pulse counts data capture
#
# (C) 2022-2023 Gissio
#
# License: MIT
#
# Notes:
# * To get the address for the 'dose' variable,
#   compile the firmware and run this command:
#
#   objdump -t platform.io/.pio/build/fs2011/firmware.elf | grep dose
#

from datetime import datetime
import swd
import time

# Configuration
dose = 0x20000418

# Variables
dev = None

doseTime = dose + 1 * 4
dosePulseCount = dose + 2 * 4

snapshotLastCount = None
snapshotLastTime = None

file = open('pulsecounts-data.csv', 'w')

while(True):
    # Get data
    if dev == None:
        dev = swd.Swd()

    try:
        snapshotTime = dev.get_mem32(doseTime)
        snapshotCount = dev.get_mem32(dosePulseCount)

        # Process data
        if snapshotLastTime != snapshotTime:
            print('.', end='', flush=True)

            if snapshotLastCount == None:
                snapshotLastCount = snapshotCount 

            deltaCount = (snapshotCount - snapshotLastCount) & 0xffffffff

            snapshotLastCount = snapshotCount
            snapshotLastTime = snapshotTime

            timestamp = datetime.now().strftime('%Y-%m-%dT%H:%M:%S.%f')
            file.write('"' + timestamp + '", ' + str(deltaCount) + '\n')
            file.flush()

        # Wait
        time.sleep(0.5)

    except Exception as e:
        print(e)

        dev = None

        time.sleep(5)
