# Rad Pro
# Python tool
#
# (C) 2022-2023 Gissio
#
# License: MIT
#

import argparse
from datetime import datetime, timezone
import math
import requests
import serial
import swd
import time

# https://gmcmap.com radiation monitoring website credentials:
gmcmap_user_account_id = ''
gmcmap_geiger_counter_id = ''

# https://radmon.org radiation monitoring website credentials:
radmon_username = ''
radmon_data_sending_password = ''

# https://safecast.org radiation monitoring website credentials:
safecast_api_key = ''
safecast_device_id = 264  # FS2011: 264 / Bosean FS-600: 265 / Bosean FS-1000: 268
safecast_location_latitude = 0
safecast_location_longitude = 0
safecast_location_height = 0


class RadProDevice:
    def __init__(self, port):
        self.port = port

        self.serial = None
        self.swd = None

    def open(self):
        if self.port == 'SWD':
            self.swd = swd.Swd()

            radpro_id_address = 0x08000020
            swd_comm_address = 0x08000024

            radpro_id = self.swd.get_mem32(radpro_id_address)
            if radpro_id != 0x50444152:
                raise Exception("Rad Pro not installed on device.")

            comm_address = self.swd.get_mem32(swd_comm_address)
            self.comm_port_address = comm_address + 0x0
            self.comm_state_address = comm_address + 0x1
            self.comm_buffer_index_address = comm_address + 0x4
            self.comm_buffer_address = comm_address + 0x8

        else:
            self.serial = serial.Serial(port=self.port,
                                        baudrate=115200,
                                        timeout=5)

    def wait_while_comm_state(self, comm_state_dest):
        end_time = time.time() + 5

        while True:
            comm_state = bytes(self.swd.read_mem(
                self.comm_state_address, 1))[0]

            if comm_state != comm_state_dest:
                return comm_state

            if time.time() > end_time:
                raise Exception('SWD request timed out.')

    def query(self, request):
        success = False
        response = None

        while True:
            try:
                if self.port == 'SWD':
                    if self.swd == None:
                        self.open()

                    self.swd.write_mem(
                        self.comm_port_address, b'\x01')  # COMM_SWD
                    self.swd.write_mem(
                        self.comm_buffer_address, request.encode('ascii') + b'\0')
                    self.swd.write_mem(
                        self.comm_state_address, b'\x01')  # COMM_RX_READY
                    comm_state = self.wait_while_comm_state(1)  # COMM_RX_READY

                    response_bytes = b''

                    while comm_state == 2:  # COMM_TX
                        response_data = bytes(self.swd.read_mem(
                            self.comm_buffer_address, 80))
                        response_bytes += response_data.split(b'\x00', 1)[0]

                        self.swd.write_mem(
                            self.comm_state_address, b'\x03')  # COMM_TX_READY
                        comm_state = self.wait_while_comm_state(
                            3)  # COMM_TX_READY

                else:
                    if self.serial == None:
                        self.open()

                    self.serial.write(request.encode('ascii') + b'\n')

                    response_bytes = self.serial.readline()

                response = response_bytes.decode('ascii').strip()

                if response.startswith('OK'):
                    success = True
                    response = response[3:]

                break

            except Exception as e:
                print(str(e))

                time.sleep(5)

                self.serial = None
                self.swd = None

        time.sleep(0.05)

        return (success, response)


def get_property(device, property):
    success, response = device.query("GET " + str(property))
    if not success:
        print(f'Error while getting property "{property}".')

        response = None

    return response


def show_property(device, property):
    value = get_property(device, property)
    if value != None:
        print(f"{property}: " + value)


def reset_life_stats(device):
    success, _ = device.query("SET tubeTime 0")
    if not success:
        print('Error while setting device life timer.')

    success, _ = device.query("SET tubePulseCount 0")
    if not success:
        print('Error while setting device life pulse count.')


def sync_time(device):
    timestamp = int(datetime.now().replace(tzinfo=timezone.utc).timestamp())

    success, _ = device.query("SET deviceTime " + str(timestamp))
    if not success:
        print('Error while setting device time.')


def download_log(device, path):
    success, response = device.query("GET datalog")

    if success:
        lines = [line + '\n' for line in response.split(';')]

        open(path, 'w').writelines(lines)

    else:
        print('Error while downloading data log.')


def submit_data(url, method='get', json=None):
    try:
        if method == 'get':
            requests.get(url=url, timeout=60)

        elif method == 'post':
            requests.post(url=url, json=json, timeout=60)

    except Exception as e:
        print('Error while submitting data: ' + str(e))


def live_log(device, args):
    success, response = device.query("GET tubeConversionFactor")

    if not success:
        print('Error: while getting tube conversion factor.')
        return

    conversion_factor = float(response)

    last_time = time.time()
    last_pulse_count = None

    log_cpm_data = args.log_pulse_counts_file != None or args.log_gmcmap or args.log_radmon or args.log_safecast

    while True:
        if log_cpm_data:
            success, response = device.query("GET tubePulseCount")

            # Process response
            if success:
                capture_datetime = datetime.now()

                entry_pulse_count = int(response)

                if last_pulse_count == None:
                    last_pulse_count = entry_pulse_count

                else:
                    delta_pulse_count = entry_pulse_count - last_pulse_count
                    last_pulse_count = entry_pulse_count

                    # Calculate cpm & uSv/h
                    cpm = delta_pulse_count * (60 / args.log_period)
                    uSvH = cpm / conversion_factor

                    if args.log_pulse_counts_file != None:
                        open(args.log_pulse_counts_file, 'at').write(
                            f'"{capture_datetime}", {entry_pulse_count}\n')

                    if args.log_gmcmap:
                        submit_data(url='https://www.gmcmap.com/log2.asp' +
                                    f'?AID={gmcmap_user_account_id}' +
                                    f'&GID={gmcmap_geiger_counter_id}' +
                                    f'&CPM={cpm:.0f}' +
                                    f'&ACPM={cpm:.3f}' +
                                    f'&uSV={uSvH:.3f}'
                                    )

                    if args.log_radmon:
                        submit_data(url='https://radmon.org/radmon.php?function=submit' +
                                    f'&user={radmon_username}' +
                                    f'&password={radmon_data_sending_password}' +
                                    f'&value={cpm:.3f}' +
                                    '&unit=CPM')

                    if args.log_safecast:
                        json = {
                            "value": f'{cpm:.3f}',
                            "unit": "cpm",
                            "device_id": safecast_device_id,
                            "captured_at": f'"{capture_datetime.astimezone(timezone.utc).isoformat()}"',
                            "latitude": f'{safecast_location_latitude:.6f}',
                            "longitude": f'{safecast_location_longitude:.6f}',
                            "height": f'{safecast_location_height:.0f}',
                        }

                        submit_data(method='post',
                                    url='https://api.safecast.org/measurements.json?api_key=' + safecast_api_key,
                                    json=json)

            else:
                print('Error while reading data: ' + response)

        # Wait
        delta_time = time.time() - last_time
        delta_time_frac = args.log_period * \
            math.ceil(delta_time / args.log_period) - delta_time

        if args.log_rng_file:
            next_time = time.time() + delta_time_frac

            while time.time() < next_time:
                success, response = device.query("GET entropy")

                if success and len(response) > 0:
                    try:
                        data = bytes.fromhex(response)
                        open(args.log_rng_file, 'ab').write(data)

                    except:
                        pass

                time.sleep(0.05)

        else:
            time.sleep(delta_time_frac)

        last_time += args.log_period


parser = argparse.ArgumentParser(
    description='Tool for interfacing with a Rad Pro device.')
parser.add_argument('-p', '--port',
                    dest='port',
                    required=True,
                    help='port (serial port device id or "SWD")')
parser.add_argument('--get-device-id',
                    action='store_true',
                    help='get device id')
parser.add_argument('--no-sync-time',
                    action='store_true',
                    help='do not synchronize device\'s date and time')
parser.add_argument('--reset-life-stats',
                    action='store_true',
                    help='reset tube life time and pulse count')
parser.add_argument('--log-pulse-counts',
                    dest='log_pulse_counts_file',
                    help='live log counts data to a file')
parser.add_argument('--log-rng',
                    dest='log_rng_file',
                    help='live log random number generator (RNG) data to a file')
parser.add_argument('--log-gmcmap',
                    action='store_true',
                    help='live log to https://gmcmap.com')
parser.add_argument('--log-radmon',
                    action='store_true',
                    help='live log to https://radmon.org')
parser.add_argument('--log-safecast',
                    action='store_true',
                    help='live log to https://safecast.org')
parser.add_argument('--log-period',
                    type=int,
                    choices=range(1, 3600),
                    metavar='[1-3600]',
                    default=300,
                    help='sets the live logging period in seconds (default: 300 seconds)')
parser.add_argument('--download-datalog',
                    dest='datalog_file',
                    help='download the datalog to a file')
parser.add_argument('--get-tube-rate',
                    action='store_true',
                    help='get tube instantaneous rate (cpm)')
parser.add_argument('--get-tube-conversion-factor',
                    action='store_true',
                    help='get tube conversion factor')
parser.add_argument('--get-tube-dead-time-compensation',
                    action='store_true',
                    help='get tube dead-time compensation factor')
parser.add_argument('--get-tube-hv-duty-cycle',
                    action='store_true',
                    help='get tube PWM duty cycle of the high-voltage generator')
parser.add_argument('--get-tube-hv-frequency',
                    action='store_true',
                    help='get tube PWM frequency of the high-voltage generator')
parser.add_argument('--get-tube-dead-time',
                    action='store_true',
                    help='get tube dead-time upper bound measurement')

args = parser.parse_args()

device = RadProDevice(args.port)

try:
    device.open()

except Exception as e:
    if str(e) == '':
        e = 'Could not open port.'

    print("Error: " + str(e))
    exit(1)

if not args.no_sync_time:
    if not args.get_device_id and\
            not args.get_tube_rate and\
            not args.get_tube_hv_duty_cycle and\
            not args.get_tube_hv_frequency and\
            not args.get_tube_conversion_factor and\
            not args.get_tube_dead_time and\
            not args.get_tube_dead_time_compensation:
        print("Syncing time...")

    sync_time(device)

if args.reset_life_stats:
    print("Resettings device life stats...")

    reset_life_stats(device)

if args.get_device_id:
    show_property(device, 'deviceId')

if args.get_tube_rate:
    show_property(device, 'tubeRate')

if args.get_tube_conversion_factor:
    show_property(device, 'tubeConversionFactor')

if args.get_tube_dead_time_compensation:
    show_property(device, 'tubeDeadTimeCompensation')

if args.get_tube_hv_duty_cycle:
    show_property(device, 'tubeHVDutyCycle')

if args.get_tube_hv_frequency:
    show_property(device, 'tubeHVFrequency')

if args.get_tube_dead_time:
    show_property(device, 'tubeDeadTime')

if args.datalog_file:
    print("Downloading offline data...")

    download_log(device, args.datalog_file)

if args.log_pulse_counts_file != None or\
        args.log_rng_file != None or\
        args.log_gmcmap or\
        args.log_radmon or\
        args.log_safecast:
    print("Logging live data...")

    live_log(device, args)
