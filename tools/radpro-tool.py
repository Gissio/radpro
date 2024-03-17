# Rad Pro
# Python tool
#
# (C) 2022-2023 Gissio
#
# License: MIT
#

import argparse
from datetime import datetime
import math
import requests
import serial
import swd
import sys
import time


# Definitions

radpro_tool_version = '1.0.1'


# Code

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
                raise Exception('Rad Pro not installed on device.')

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
    success, response = device.query('GET ' + str(property))
    if not success:
        print(f'Error while getting property "{property}".')

        response = None

    return response


def show_property(device, property):
    value = get_property(device, property)
    if value != None:
        print(f'{property}: ' + value)


def reset_tube_life_stats(device):
    success, _ = device.query('SET tubeTime 0')
    if not success:
        print('Error while setting device life timer.')

    success, _ = device.query('SET tubePulseCount 0')
    if not success:
        print('Error while setting device life pulse count.')


def sync_time(device):
    timestamp = int(datetime.now().timestamp())

    success, _ = device.query('SET deviceTime ' + str(timestamp))
    if not success:
        print('Error while setting device time.')


def download_log(device, path, start_datetime):
    if (start_datetime != None):
        start_time = int(datetime.fromisoformat(
            start_datetime).timestamp())
        success, response = device.query('GET datalog ' + str(start_time))
    else:
        success, response = device.query('GET datalog')

    if success:
        records = response.split(';')
        lines = []

        for index, record in enumerate(records):
            if index == 0:
                continue

            values = record.split(',')
            record_time = datetime.fromtimestamp(
                int(values[0])).isoformat()
            lines.append(f'"{record_time}",{values[1]}\n')

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


def log_live_data(device, args):
    success, response = device.query('GET tubeConversionFactor')

    if not success:
        print('Error: while getting tube conversion factor.')
        return

    conversion_factor = float(response)

    last_time = time.time()
    last_pulse_count = None

    log_cpm_data = args.pulsecount_file != None or\
        args.submit_gmcmap != None or\
        args.submit_radmon != None or\
        args.submit_safecast != None

    while True:
        if log_cpm_data:
            success, response = device.query('GET tubePulseCount')

            # Process response
            if success:
                capture_datetime = datetime.now().isoformat()

                entry_pulse_count = int(response)

                if last_pulse_count == None:
                    last_pulse_count = entry_pulse_count

                else:
                    delta_pulse_count = entry_pulse_count - last_pulse_count
                    last_pulse_count = entry_pulse_count

                    # Calculate cpm & uSv/h
                    cpm = delta_pulse_count * (60 / args.capture_period)
                    uSvH = cpm / conversion_factor

                    if args.pulsecount_file != None:
                        open(args.pulsecount_file, 'at').write(
                            f'"{capture_datetime}",{entry_pulse_count}\n')

                    if args.submit_gmcmap != None:
                        submit_data(url='https://www.gmcmap.com/log2.asp' +
                                    f'?AID={args.submit_gmcmap[0]}' +
                                    f'&GID={args.submit_gmcmap[1]}' +
                                    f'&CPM={cpm:.0f}' +
                                    f'&ACPM={cpm:.3f}' +
                                    f'&uSV={uSvH:.3f}'
                                    )

                    if args.submit_radmon != None:
                        submit_data(url='https://radmon.org/radmon.php?function=submit' +
                                    f'&user={args.submit_radmon[0]}' +
                                    f'&password={args.submit_radmon[1]}' +
                                    f'&value={cpm:.3f}' +
                                    '&unit=CPM')

                    if args.submit_safecast != None:
                        json = {
                            'value': f'{cpm:.3f}',
                            'unit': 'cpm',
                            'device_id': args.submit_safecast[1],
                            'captured_at': f'"{capture_datetime}"',
                            'latitude': '0',
                            'longitude': '0',
                            'height': '0',
                        }

                        submit_data(method='post',
                                    url='https://api.safecast.org/measurements.json?api_key=' +
                                        args.submit_safecast[0],
                                    json=json)

            else:
                print('Error while reading data: ' + response)

        # Wait
        delta_time = time.time() - last_time
        delta_time_frac = args.capture_period * \
            math.ceil(delta_time / args.capture_period) - delta_time

        if args.randomdata_file:
            next_time = time.time() + delta_time_frac

            while time.time() < next_time:
                success, response = device.query('GET randomData')

                if success and len(response) > 0:
                    try:
                        data = bytes.fromhex(response)
                        open(args.randomdata_file, 'ab').write(data)

                    except:
                        pass

                time.sleep(0.05)

        else:
            time.sleep(delta_time_frac)

        last_time += args.capture_period


parser = argparse.ArgumentParser(
    description='Tool for interfacing with a Rad Pro device.')
parser.add_argument('--version',
                    dest='version',
                    action='store_true',
                    help='print version information')
parser.add_argument('-p', '--port',
                    dest='port',
                    help='serial port device id or "SWD" (e.g. "COM13" or "/dev/ttyS0)"')
parser.add_argument('--download-datalog',
                    dest='datalog_file',
                    help='download datalog to a .csv file')
parser.add_argument('--download-datalog-start',
                    dest='datalog_start_datetime',
                    help='limit datalog download to entries newer than a certain ISO 8601 date and time (e.g. "2024-01-01" or "2024-01-01T12:00:00Z")')
parser.add_argument('--log-pulse-count',
                    dest='pulsecount_file',
                    help='live log pulse counts to a .csv file')
parser.add_argument('--submit-gmcmap',
                    nargs=2,
                    metavar=('USER_ACCOUNT_ID', 'GEIGER_COUNTER_ID'),
                    action='store',
                    help='live submit data to https://gmcmap.com')
parser.add_argument('--submit-radmon',
                    nargs=2,
                    metavar=('USERNAME', 'DATA_SENDING_PASSWORD'),
                    action='store',
                    help='live submit data to https://radmon.org')
parser.add_argument('--submit-safecast',
                    nargs=2,
                    metavar=('API_KEY', 'DEVICE_ID'),
                    action='store',
                    help='live submit data to https://safecast.org')
parser.add_argument('--capture-period',
                    type=int,
                    choices=range(1, 3600),
                    metavar='[1-3600]',
                    default=300,
                    help='sets the time period for live logging pulse counts and data submission to radiation monitoring websites (default: 300 seconds)')
parser.add_argument('--log-randomdata',
                    dest='randomdata_file',
                    help='live log random generator data to a binary file')
parser.add_argument('--get-device-id',
                    action='store_true',
                    help='get device id')
parser.add_argument('--get-device-battery-voltage',
                    action='store_true',
                    help='get device battery voltage (per cell)')
parser.add_argument('--no-sync-time',
                    action='store_true',
                    help='do not synchronize device\'s date and time')
parser.add_argument('--get-tube-time',
                    action='store_true',
                    help='get tube life time in seconds')
parser.add_argument('--get-tube-pulse-count',
                    action='store_true',
                    help='get tube life pulse count')
parser.add_argument('--reset-tube-life-stats',
                    action='store_true',
                    help='reset tube life time and pulse count')
parser.add_argument('--get-tube-rate',
                    action='store_true',
                    help='get tube instantaneous rate (in cpm)')
parser.add_argument('--get-tube-conversion-factor',
                    action='store_true',
                    help='get tube conversion factor')
parser.add_argument('--get-tube-dead-time',
                    action='store_true',
                    help='get measurement of the tube\'s dead time')
parser.add_argument('--get-tube-dead-time-compensation',
                    action='store_true',
                    help='get tube dead-time compensation in seconds')
parser.add_argument('--get-tube-hv-duty-cycle',
                    action='store_true',
                    help='get tube PWM duty cycle of the high voltage generator')
parser.add_argument('--get-tube-hv-frequency',
                    action='store_true',
                    help='get tube PWM frequency of the high voltage generator')

args = parser.parse_args()

if args.version:
    print('radpro-tool ' + radpro_tool_version)
    sys.exit(0)

if not args.port:
    parser.print_usage()
    print('Error: the following arguments are required: -p/--port')
    sys.exit(1)

device = RadProDevice(args.port)

try:
    device.open()

except Exception as e:
    if str(e) == '':
        e = 'Could not open port.'

    print('Error: ' + str(e))
    sys.exit(1)

if args.get_device_id:
    show_property(device, 'deviceId')

if args.get_device_battery_voltage:
    show_property(device, 'deviceBatteryVoltage')

if not args.no_sync_time:
    if not args.get_device_id and\
            not args.get_device_battery_voltage and\
            not args.get_tube_rate and\
            not args.get_tube_hv_duty_cycle and\
            not args.get_tube_hv_frequency and\
            not args.get_tube_conversion_factor and\
            not args.get_tube_dead_time and\
            not args.get_tube_dead_time_compensation:
        print('Syncing time...')

    sync_time(device)

if args.get_tube_time:
    show_property(device, 'tubeTime')

if args.get_tube_pulse_count:
    show_property(device, 'tubePulseCount')

if args.reset_tube_life_stats:
    print('Resettings tube life stats...')

    reset_tube_life_stats(device)

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
    print('Downloading offline data...')

    download_log(device, args.datalog_file, args.datalog_start_datetime)

if args.pulsecount_file != None or\
        args.randomdata_file != None or\
        args.submit_gmcmap != None or\
        args.submit_radmon != None or\
        args.submit_safecast != None:
    print('Logging live data...')

    log_live_data(device, args)
