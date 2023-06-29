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

# Rad Pro variables
gm = 0x20000394

# Local variables
gm_pulses_queue_head_tail = gm
gm_pulses_queue = gm + 4
gm_pulses_queue_size = 16
gm_pulses_queue_mask = gm_pulses_queue_size - 1

dev = None

pulses_queue = [0] * gm_pulses_queue_size
pulses_queue_head_tail = None
pulses_queue_tail = None
last_pulse_time = None

file = open('pulseintervals-data.bin', 'wb')

while(True):
    # Get data
    try:
        if dev == None:
            dev = swd.Swd()

        pulses_queue_head_tail = dev.get_mem32(gm_pulses_queue_head_tail)
        for i in range(0, gm_pulses_queue_size):
            pulses_queue[i] = dev.get_mem32(gm_pulses_queue + 4 * i)

    except Exception as e:
        print()
        print('Exception ' + str(e.__class__) + ' ' + str(e))

        dev = None

        time.sleep(5)

    # Process data
    pulses_queue_head = pulses_queue_head_tail & gm_pulses_queue_mask
    if pulses_queue_tail == None:
        pulses_queue_tail = pulses_queue_head

    last_pulses_queue_tail = pulses_queue_tail

    while pulses_queue_tail != pulses_queue_head:
        pulseTime = pulses_queue[pulses_queue_tail]

        if last_pulse_time != None:
            interval = (pulseTime - last_pulse_time) & 0xffffffff
            file.write(int.to_bytes(interval, 4))

        last_pulse_time = pulseTime
        pulses_queue_tail = (pulses_queue_tail + 1) & gm_pulses_queue_mask

    if last_pulses_queue_tail != pulses_queue_head:
        file.flush()

    # Wait
    time.sleep(0.1)
