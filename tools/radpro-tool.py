# Rad Pro
# Python tool
#
# (C) 2022-2025 Gissio
#
# License: MIT
#

import argparse
from datetime import datetime
import math
import os
import requests
import serial
import swd
import sys
import time


# Definitions

radpro_tool_version = '1.0.2'
log_warnings = False


# Errors

def log(level, message):
    print(f'{datetime.now().isoformat()} {level}: {message}')


def log_info(message):
    log('info', message)


def log_warning(message):
    global log_warnings
    log_warnings = True

    log('warning', message)


def log_error(message):
    log('error', message)

    sys.exit(1)


# Rad Pro I/O class

class RadProIO:
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
                raise Exception('Rad Pro not installed on device')

            comm_address = self.swd.get_mem32(swd_comm_address)
            self.comm_port_address = comm_address + 0x0
            self.comm_state_address = comm_address + 0x1
            self.comm_buffer_index_address = comm_address + 0x4
            self.comm_buffer_address = comm_address + 0x8

        else:
            self.serial = serial.Serial(port=self.port,
                                        baudrate=115200,
                                        timeout=0.5,
                                        write_timeout=0.5)

    def wait_while_comm_state(self, comm_state_dest):
        end_time = time.time() + 5

        while True:
            comm_state = bytes(self.swd.read_mem(
                self.comm_state_address, 1))[0]

            if comm_state != comm_state_dest:
                return comm_state

            if time.time() > end_time:
                raise Exception('SWD request timed out')

    def query(self, request):
        response = None
        response_bytes = None

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

        except Exception as e:
            log_warning(e)

            time.sleep(5)

            self.serial = None
            self.swd = None

        time.sleep(0.05)

        if response_bytes == None:
            return None

        response = response_bytes.decode('ascii').strip()

        if response.startswith('OK'):
            return response[3:]
        else:
            log_warning(f'request "{request}": response "{response}"')

            return None

    def get(self, key):
        return self.query(f'GET {key}')

    def set(self, key, value):
        return self.query(f'SET {key} {value}')


# Helper functions

def print_property(io, key):
    value = io.get(key)

    if value != None:
        print(f'{key}:{value}')


def reset_tube_life_stats(io):
    io.set('tubeTime', 0)
    io.set('tubePulseCount', 0)


def sync_time(io):
    current_time = int(time.time())
    if time.localtime().tm_isdst:
        current_timezone = time.altzone
    else:
        current_timezone = time.timezone

    io.set('deviceTime', current_time)
    io.set('deviceTimeZone', int(-current_timezone * 10 / 3600) / 10)


def get_sensitivity(io):
    response = io.get('tubeSensitivity')

    if response != None:
        try:
            return float(response)
        except:
            log_error(
                f'could not decode tube sensitivity: response "f{response}"')

    log_error(f'could not get tube sensitivity')

    return None


def get_datalog(io, start_datetime):
    if start_datetime != None:
        start_time = int(datetime.fromisoformat(
            start_datetime).timestamp())
        return io.get(f'datalog {start_time}')
    else:
        return io.get('datalog')


def get_pulsecount(io):
    response = io.get('tubePulseCount')

    if response != None:
        try:
            return int(response)
        except:
            log_warning(f'could not decode pulse count: "{response}"')

    return None


def get_randomdata(io):
    response = io.get('randomData')

    if response != None and len(response) > 0:
        try:
            return bytes.fromhex(response)
        except:
            log_warning(f'could not decode random data: "{response}"')

    return None


def download_datalog(io, path, start_datetime):
    sensitivity = get_sensitivity(io)

    datalog = get_datalog(io, start_datetime)
    if datalog == None:
        return

    records = datalog.split(';')
    lines = []

    prev_timestamp = None
    prev_pulsecount = None
    prev_deltatime = None

    for index, record in enumerate(records):
        # Ignore header
        if index == 0:
            continue

        values = record.split(',')

        if len(values) != 2:
            log_warning(f'invalid record: "{record}"')
            continue

        try:
            curr_timestamp = int(values[0])
            curr_datetime = str(datetime.fromtimestamp(curr_timestamp))
        except Exception as e:
            log_warning(
                f'could not decode timestamp: record "{record}": {e}')
            continue

        try:
            curr_pulsecount = int(values[1])
        except Exception as e:
            log_warning(
                f'could not decode pulse count: record "{record}": {e}')
            continue

        cpm = None
        if prev_timestamp != None:
            curr_deltatime = curr_timestamp - prev_timestamp

            if curr_deltatime < 0:
                log_warning(f'time moving backwards: record "{record}"')
            else:
                delta_pulsecount = curr_pulsecount - prev_pulsecount
                if delta_pulsecount < 0:
                    log_warning(
                        f'pulse count moving backwards: record "{record}"')

                if prev_deltatime != None and prev_deltatime < curr_deltatime:
                    log_info(f'time gap {curr_deltatime} s: record "{record}"')
                else:
                    if curr_deltatime > 0:
                        cpm = delta_pulsecount * 60 / curr_deltatime
                        uSvH = cpm / sensitivity

            prev_deltatime = curr_deltatime

        prev_timestamp = curr_timestamp
        prev_pulsecount = curr_pulsecount

        if cpm != None:
            lines.append(
                f'{curr_timestamp},{curr_datetime},{curr_pulsecount},{cpm:.1f},{uSvH:.3f}\n')
        else:
            lines.append(
                f'{curr_timestamp},{curr_datetime},{curr_pulsecount},,\n')

    try:
        f = open(path, 'w')
        f.write('# timestamp,date time,pulse count,cpm,uSvH\n')
        f.writelines(lines)
    except Exception as e:
        log_warning(f'could not write {path}')


def send_http_request(url, method='get', json=None):
    try:
        if method == 'get':
            requests.get(url=url, timeout=60)
        elif method == 'post':
            requests.post(url=url, json=json, timeout=60)
    except Exception as e:
        log_warning(f'could not submit HTTP data: url "{url}": {e}')


def stream_datalog(io, args):
    sensitivity = get_sensitivity(io)

    log_pulsecounts = args.pulsedata_file != None or\
        args.submit_gmcmap != None or\
        args.submit_radmon != None or\
        args.submit_safecast != None

    if args.pulsedata_file != None:
        if not os.path.exists(args.pulsedata_file):
            try:
                open(args.pulsedata_file, 'wt').write(
                    '# timestamp,date time,pulse count,cpm,uSvH\n')
            except Exception as e:
                log_error(
                    f'could not create file: "{args.pulsedata_file}": {e}')

    next_event = int(time.time())

    prev_timestamp = None
    prev_pulsecount = None

    while True:
        # Measurement
        if log_pulsecounts:
            curr_pulsecount = get_pulsecount(io)
            curr_timestamp = int(time.time())

            if curr_pulsecount == None:
                while next_event < curr_timestamp:
                    next_event += args.period

            cpm = None
            if curr_pulsecount != None and prev_pulsecount != None:
                curr_deltatime = curr_timestamp - prev_timestamp

                delta_pulsecount = curr_pulsecount - prev_pulsecount
                if delta_pulsecount < 0:
                    log_warning(
                        f'pulse count moving backwards: {curr_pulsecount}')

                if curr_deltatime > 0:
                    cpm = delta_pulsecount * 60 / curr_deltatime
                    uSvH = cpm / sensitivity

            prev_timestamp = curr_timestamp
            prev_pulsecount = curr_pulsecount

            if args.pulsedata_file != None and curr_pulsecount != None:
                curr_datetime = str(datetime.fromtimestamp(curr_timestamp))
                if cpm != None:
                    line = f'{curr_timestamp},{curr_datetime},{curr_pulsecount},{cpm:.1f},{uSvH:.3f}\n'
                else:
                    line = f'{curr_timestamp},{curr_datetime},{curr_pulsecount},,\n'

                try:
                    open(args.pulsedata_file, 'at').write(line)
                except Exception as e:
                    log_error(
                        f'could not write file: "{args.pulsedata_file}": {e}')

            if args.submit_gmcmap != None and cpm != None:
                url = 'https://www.gmcmap.com/log2.asp' + \
                    f'?AID={args.submit_gmcmap[0]}' + f'&GID={args.submit_gmcmap[1]}' + \
                    f'&CPM={cpm:.0f}' + f'&ACPM={cpm:.3f}' + \
                    f'&uSV={uSvH:.3f}'
                send_http_request(url)

            if args.submit_radmon != None and cpm != None:
                url = 'https://radmon.org/radmon.php?function=submit' + \
                    f'&user={args.submit_radmon[0]}' + \
                    f'&password={args.submit_radmon[1]}' + \
                    f'&value={cpm:.3f}' + '&unit=CPM'
                send_http_request(url)

            if args.submit_safecast != None and cpm != None:
                url = 'https://api.safecast.org/measurements.json?api_key=' + \
                    args.submit_safecast[0]
                json = {
                    'value': f'{cpm:.3f}',
                    'unit': 'cpm',
                    'device_id': args.submit_safecast[1],
                    'captured_at': f'"{curr_datetime.isoformat()}"',
                    'latitude': '0',
                    'longitude': '0',
                    'height': '0',
                }
                send_http_request(url, 'post', json)

        # Wait for next measurement
        next_event += args.period

        if not args.randomdata_file:
            sleep_time = next_event - time.time()
            if sleep_time > 0:
                time.sleep(sleep_time)

        else:
            while time.time() < next_event:
                data = get_randomdata(io)

                if data != None:
                    try:
                        open(args.randomdata_file, 'ab').write(data)
                    except Exception as e:
                        log_error(
                            f'could not write file: "{args.randomdata_file}": {e}')

                time.sleep(0.05)


# Main

def main():
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
                        help='download data log to a .csv file')
    parser.add_argument('--download-datalog-start',
                        dest='datalog_datetime',
                        help='download only data log entries newer than a certain ISO 8601 date and time (e.g. "2000-01-01" or "2000-01-01T12:00:00")')
    parser.add_argument('--log-pulsedata',
                        dest='pulsedata_file',
                        help='log live pulse data to a .csv file')
    parser.add_argument('--period',
                        type=int,
                        choices=range(1, 3600),
                        metavar='[1-3600]',
                        default=300,
                        help='sets the time period for logging pulse data live and submitting data live to radiation monitoring websites (default: 300 seconds)')
    parser.add_argument('--submit-gmcmap',
                        nargs=2,
                        metavar=('USER_ACCOUNT_ID', 'GEIGER_COUNTER_ID'),
                        action='store',
                        help='submit live data to https://gmcmap.com')
    parser.add_argument('--submit-radmon',
                        nargs=2,
                        metavar=('USERNAME', 'DATA_SENDING_PASSWORD'),
                        action='store',
                        help='submit live data to https://radmon.org')
    parser.add_argument('--submit-safecast',
                        nargs=2,
                        metavar=('API_KEY', 'DEVICE_ID'),
                        action='store',
                        help='submit live data to https://safecast.org')
    parser.add_argument('--log-randomdata',
                        dest='randomdata_file',
                        help='log live randomly generated data to a binary file')
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
    parser.add_argument('--get-tube-sensitivity',
                        action='store_true',
                        help='get tube sensitivity')
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
        log_error('the following arguments are required: -p/--port')

    io = RadProIO(args.port)

    try:
        io.open()

    except Exception as e:
        if str(e) == '':
            e = 'could not open port'

        log_error(e)

    if args.get_device_id:
        print_property(io, 'deviceId')

    if args.get_device_battery_voltage:
        print_property(io, 'deviceBatteryVoltage')

    if not args.no_sync_time:
        if not args.get_device_id and\
                not args.get_device_battery_voltage and\
                not args.get_tube_rate and\
                not args.get_tube_hv_duty_cycle and\
                not args.get_tube_hv_frequency and\
                not args.get_tube_sensitivity and\
                not args.get_tube_dead_time and\
                not args.get_tube_dead_time_compensation:
            print('Syncing time...')

        sync_time(io)

    if args.get_tube_time:
        print_property(io, 'tubeTime')

    if args.get_tube_pulse_count:
        print_property(io, 'tubePulseCount')

    if args.reset_tube_life_stats:
        print('Resettings tube life stats...')

        reset_tube_life_stats(io)

    if args.get_tube_rate:
        print_property(io, 'tubeRate')

    if args.get_tube_sensitivity:
        print_property(io, 'tubeSensitivity')

    if args.get_tube_dead_time:
        print_property(io, 'tubeDeadTime')

    if args.get_tube_dead_time_compensation:
        print_property(io, 'tubeDeadTimeCompensation')

    if args.get_tube_hv_duty_cycle:
        print_property(io, 'tubeHVDutyCycle')

    if args.get_tube_hv_frequency:
        print_property(io, 'tubeHVFrequency')

    if args.datalog_file:
        print('Downloading data log...')

        download_datalog(io,
                         args.datalog_file,
                         args.datalog_datetime)

    if args.pulsedata_file != None or\
            args.randomdata_file != None or\
            args.submit_gmcmap != None or\
            args.submit_radmon != None or\
            args.submit_safecast != None:
        print('Logging live...')

        stream_datalog(io, args)

    if log_warnings:
        sys.exit(2)


if __name__ == '__main__':
    main()
