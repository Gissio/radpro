# Rad Pro
# Battery discharge capture
#
# (C) 2022-2023 Gissio
#
# License: MIT
#
# Notes:
# * To get the addresses for the 'battery and 'dose' variables,
#   compile the firmware and run these commands:
#
#   objdump -t platform.io/.pio/build/fs2011/firmware.elf | grep battery
#   objdump -t platform.io/.pio/build/fs2011/firmware.elf | grep dose
#

from datetime import datetime
import swd
import time

# Rad Pro variables
battery = 0x20000000
dose = 0x20000000

# Local variables
battery_value = battery
dose_time = dose + 4

dev = None

snapshot_time = None
snapshot_value = None
snapshot_last_time = None

file = open('batterydischarge-data.csv', 'a')

while True:
    # Get data
    try:
        if dev == None:
            dev = swd.Swd()

        snapshot_time = dev.get_mem32(dose_time)
        snapshot_value = dev.get_mem32(battery_value)

    except Exception as e:
        print()
        print('Exception ' + str(e.__class__) + ' ' + str(e))

        dev = None

        time.sleep(5)

    # Process data
    if snapshot_last_time != snapshot_time:
        snapshot_last_time = snapshot_time

        timestamp = datetime.now().strftime('%Y-%m-%dT%H:%M:%S.%f')

        file.write('"' + timestamp + '", ' + str(snapshot_value) + '\n')
        file.flush()

    # Wait
    time.sleep(0.5)
