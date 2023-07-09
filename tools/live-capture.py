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

# Rad Pro variables
dose = 0x20000464

# Local variables
dose_time = dose + 1 * 4
dose_count = dose + 2 * 4

dev = None

snapshot_last_time = None
snapshot_last_count = None

file = open('live-capture.csv', 'w')

while True:
    # Get data
    try:
        if dev == None:
            dev = swd.Swd()

        snapshot_time = dev.get_mem32(dose_time)
        snapshot_count = dev.get_mem32(dose_count)

    except Exception as e:
        print()
        print('Exception ' + str(e.__class__) + ' ' + str(e))

        dev = None

        time.sleep(5)

    # Process data
    if snapshot_last_time != snapshot_time:
        print('.', end='', flush=True)

        if snapshot_last_count == None:
            snapshot_last_count = snapshot_count

        delta_count = (snapshot_count - snapshot_last_count) & 0xffffffff

        snapshot_last_time = snapshot_time
        snapshot_last_count = snapshot_count

        timestamp = datetime.now().strftime('%Y-%m-%dT%H:%M:%S.%f')
        file.write('"' + timestamp + '", ' + str(delta_count) + '\n')
        file.flush()

    # Wait
    time.sleep(0.5)
