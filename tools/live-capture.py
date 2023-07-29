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
signature_address = 0x08007ff0
data_address_ref = 0x08007ff8

# Local variables
dev = None

data_address = None

snapshot_last_time = None
snapshot_last_count = None

file = open('live-capture.csv', 'w')

while True:
    # Get data
    try:
        if dev == None:
            dev = swd.Swd()

            signature = dev.get_mem32(signature_address)
            if signature != 0x50444152:
                print("Abort: device invalid (signature mismatch)")
                exit(0)
            
            data_address = dev.get_mem32(data_address_ref)

        snapshot_time = dev.get_mem32(data_address + 0x00)
        snapshot_count = dev.get_mem32(data_address + 0x04)

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

        delta_count = (snapshot_count - snapshot_last_count) & 0xffff

        snapshot_last_time = snapshot_time
        snapshot_last_count = snapshot_count

        timestamp = datetime.now().strftime('%Y-%m-%dT%H:%M:%S.%f')
        file.write('"' + timestamp + '", ' + str(delta_count) + '\n')
        file.flush()

    # Wait
    time.sleep(0.5)
