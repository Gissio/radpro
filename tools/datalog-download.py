# Rad Pro
# Data logger download
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

# Rad Pro variables
dose_state_start = 0x0800a000
dose_state_end = 0x08010000

# Get data
dev = swd.Swd()

dose_time = []
dose_pulse_count = []

for i in range(dose_state_start, dose_state_end, 8):
    dose_time.append(dev.get_mem32(i))
    dose_pulse_count.append(dev.get_mem32(i + 4))

timestamp = datetime.now().strftime('%Y-%m-%dT%H_%M_%S')
file = open(timestamp + '.csv', 'w')

# Find last index
n = len(dose_time)

last_index = None

for i in range(0, n):
    if not (dose_time[i] == 0xffffffff and dose_pulse_count[i] == 0xffffffff):
        last_index = i

    elif last_index != None:
        break

if last_index == None:
    exit()

dose_time = dose_time[last_index + 1:] + dose_time[:last_index + 1]
dose_pulse_count = dose_pulse_count[last_index + 1:] + dose_pulse_count[:last_index + 1]

# Write data
dose_last_pulse_count = None

for i in range(0, n):
    if dose_time[i] == 0xffffffff and dose_pulse_count[i] == 0xffffffff:
        continue

    if dose_last_pulse_count == None:
        dose_last_pulse_count = dose_pulse_count[i]

    delta_pulse_count = dose_pulse_count[i] - dose_last_pulse_count

    file.write(str(dose_time[i]) + ', ' + str(delta_pulse_count) + '\n')

    dose_last_pulse_count = dose_pulse_count[i]
