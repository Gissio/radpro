# Rad Pro
# radmon.org client
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

import requests
import swd
import time

# Configuration
radmonUsername = 'username'
radmonPassword = 'password'

dose = 0x20000418

# Variables
dev = None

doseTime = dose + 1 * 4
dosePulseCount = dose + 2 * 4

snapshotLastTime = None

startPulseCount = None
startTime = None

while (True):
    try:
        # Read data
        if dev == None:
            dev = swd.Swd()

        snapshotPulseCount = dev.get_mem32(dosePulseCount)
        snapshotTime = dev.get_mem32(doseTime)

        # Process data
        if snapshotLastTime != snapshotTime:
            snapshotLastTime = snapshotTime

            if startTime == None:
                startPulseCount = snapshotPulseCount
                startTime = snapshotTime

            deltaPulseCount = snapshotPulseCount - startPulseCount
            deltaTime = snapshotTime - startTime

            if deltaTime > 60:
                startPulseCount = snapshotPulseCount
                startTime = snapshotTime

                # Submit data
                requests.get('https://radmon.org/radmon.php?function=submit&' +
                             f'user={radmonUsername}&' +
                             f'password={radmonPassword}&' +
                             f'value={deltaPulseCount}&' +
                             'unit=CPM')

                print('.', end='', flush=True)

        # Wait
        time.sleep(0.25)

    except Exception as e:
        print(e)

        dev = None

        time.sleep(5)
