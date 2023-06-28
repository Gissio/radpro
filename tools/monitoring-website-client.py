# Rad Pro
# gmcmap.com and radmon.org client
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
gmcmap_user_account_id = ''    # For the https://gmcmap.com radiation monitoring website
gmcmap_geiger_counter_id = ''

radmon_username = ''    # For the https://radmon.org radiation monitoring website
radmon_data_sending_password = ''

cpm_per_usv_h = 68.4    # For HH614 tube

# Rad Pro variables
dose = 0x20000418

# Local variables
dev = None

dose_time = dose + 1 * 4
dose_pulse_count = dose + 2 * 4

snapshot_last_time = None

start_pulse_count = None
start_time = None

delta_pulse_count_history = []

while (True):
    try:
        # Read data
        if dev == None:
            dev = swd.Swd()

        snapshot_pulse_count = dev.get_mem32(dose_pulse_count)
        snapshot_time = dev.get_mem32(dose_time)

        # Process data
        if snapshot_last_time != snapshot_time:
            snapshot_last_time = snapshot_time

            if start_time == None:
                start_pulse_count = snapshot_pulse_count
                start_time = snapshot_time

            delta_pulse_count = snapshot_pulse_count - start_pulse_count
            delta_time = snapshot_time - start_time

            if delta_time > 60:
                start_pulse_count = snapshot_pulse_count
                start_time = snapshot_time

                if len(delta_pulse_count_history) >= 5:
                    delta_pulse_count_history = delta_pulse_count_history[1:]
                delta_pulse_count_history.append(delta_pulse_count)

                # Submit data to https://gmcmap.com
                if gmcmap_user_account_id != '':
                    url = 'http://www.gmcmap.com/log2.asp' +\
                        f'?AID={gmcmap_user_account_id}' +\
                        f'&GID={gmcmap_geiger_counter_id}' +\
                        f'&CPM={delta_pulse_count}'

                    if len(delta_pulse_count_history) > 0:
                        averaged_delta_pulse_count = sum(delta_pulse_count_history) / len(delta_pulse_count_history)
                        uSv_h = averaged_delta_pulse_count / cpm_per_usv_h

                        url += f'&ACPM={averaged_delta_pulse_count:,.2f}' +\
                            f'&uSV={uSv_h:,.3f}'

                    requests.get(url)

                # Submit data to https://radmon.org
                if radmon_username != '':
                    requests.get('https://radmon.org/radmon.php?function=submit' +
                                 f'&user={radmon_username}' +
                                 f'&password={radmon_data_sending_password}' +
                                 f'&value={delta_pulse_count}' +
                                 '&unit=CPM')

                print('.', end='', flush=True)

        # Wait
        time.sleep(0.5)

    except Exception as e:
        print()
        print(e)

        dev = None

        time.sleep(5)
