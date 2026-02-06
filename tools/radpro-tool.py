# Rad Pro
# Python tool
#
# (C) 2022-2026 Gissio
#
# License: MIT
#

import argparse
from datetime import datetime
import math
import os
import requests
import serial
import sys
import time


# Definitions

radpro_tool_version = '2.2'
log_warnings = False


# Errors

def log(level, message):
    print(f'{datetime.now().isoformat()} {level}: {message}', file=sys.stderr)


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

    def open(self):
        self.serial = serial.Serial(port=self.port,
                                    baudrate=115200,
                                    timeout=0.5,
                                    write_timeout=0.5)

    def query(self, request):
        response = None
        response_bytes = None

        try:
            if self.serial == None:
                self.open()

            self.serial.write(request.encode('ascii') + b'\n')

            response_bytes = self.serial.readline()

        except Exception as e:
            log_warning(e)

            time.sleep(5)

            self.serial = None

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


def get_datalog(io,
                start_datetime,
                end_datetime,
                max_record_num):
    if start_datetime != None:
        start_time = int(datetime.fromisoformat(
            start_datetime).timestamp())
    else:
        start_time = 0

    if end_datetime != None:
        end_time = int(datetime.fromisoformat(
            end_datetime).timestamp())
    else:
        end_time = 4294967295

    if max_record_num != None:
        return io.get(f'datalog {start_time} {end_time} {max_record_num}')
    else:
        return io.get(f'datalog {start_time} {end_time}')


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


def download_datalog(io,
                     path,
                     start_datetime,
                     end_datetime,
                     max_record_num):
    sensitivity = get_sensitivity(io)

    datalog = get_datalog(io,
                          start_datetime,
                          end_datetime,
                          max_record_num)
    if datalog == None:
        return

    records = datalog.split(';')
    lines = []

    prev_timestamp = None
    prev_pulsecount = None

    for index, record in enumerate(records):
        # Ignore header
        if index == 0:
            continue

        # New logging session
        if record == '':
            prev_timestamp = None
            prev_pulsecount = None
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

                if curr_deltatime > 0:
                    cpm = delta_pulsecount * 60 / curr_deltatime
                    uSvH = cpm / sensitivity

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


def send_http_request(url, method='get', json=None, data=None):
    headers = {'User-Agent': 'radpro-tool/' + radpro_tool_version}

    try:
        if method == 'get':
            requests.get(url=url, timeout=60, headers=headers)
        elif method == 'post':
            if json is not None:
                requests.post(url=url, json=json, timeout=60, headers=headers)
            elif data is not None:
                requests.post(url=url, data=data, timeout=60, headers=headers)
            else:
                requests.post(url=url, timeout=60, headers=headers)
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

            curr_datetime = datetime.fromtimestamp(curr_timestamp)

            if args.pulsedata_file != None and curr_pulsecount != None:
                curr_datetime_str = str(curr_datetime)
                if cpm != None:
                    line = f'{curr_timestamp},{curr_datetime_str},{curr_pulsecount},{cpm:.1f},{uSvH:.3f}\n'
                else:
                    line = f'{curr_timestamp},{curr_datetime_str},{curr_pulsecount},,\n'

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
                send_http_request(url, 'get')
                print(
                    f'GMCMap submission: {curr_datetime} CPM:{cpm:.3f} uSv/h:{uSvH:.3f}')

            if args.submit_radmon != None and cpm != None:
                url = 'https://radmon.org/radmon.php?function=submit' + \
                    f'&user={args.submit_radmon[0]}' + \
                    f'&password={args.submit_radmon[1]}' + \
                    f'&value={cpm:.3f}' + '&unit=CPM'
                send_http_request(url)
                print(
                    f'Radmon submission: {curr_datetime} CPM:{cpm:.3f} uSv/h:{uSvH:.3f}')

            if args.submit_safecast != None and cpm != None:
                url = 'https://api.safecast.org/measurements.json?api_key=' + \
                    args.submit_safecast[0]
                json = {
                    'value': f'{cpm:.3f}',
                    'unit': 'cpm',
                    'device_id': args.submit_safecast[1],
                    'captured_at': f'"{curr_datetime.isoformat()}"',
                    'latitude': str(args.safecast_latitude),
                    'longitude': str(args.safecast_longitude),
                    'height': str(args.safecast_height),
                }
                send_http_request(url, 'post', json=json)
                print(
                    f'Safecast submission: {curr_datetime} CPM:{cpm:.3f} uSv/h:{uSvH:.3f}')

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
                        help='serial port device id (e.g. "COM13" or "/dev/ttyS0)"')
    parser.add_argument('--download-datalog',
                        dest='datalog_file',
                        help='download data log to a .csv file')
    parser.add_argument('--download-datalog-start',
                        dest='datalog_datetime_start',
                        help='download only data log records newer than a certain ISO 8601 date and time (e.g. "2000-01-01" or "2000-01-01T12:00:00")')
    parser.add_argument('--download-datalog-end',
                        dest='datalog_datetime_end',
                        help='download only data log records older than a certain ISO 8601 date and time (e.g. "2000-01-01" or "2000-01-01T12:00:00")')
    parser.add_argument('--download-datalog-max-record-num',
                        dest='datalog_max_record_num',
                        help='limit the number of data log records to download')
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
    parser.add_argument('--safecast-latitude',
                        type=float,
                        default=0.0,
                        help='decimal latitude for Safecast submissions (default: 0.0)')
    parser.add_argument('--safecast-longitude',
                        type=float,
                        default=0.0,
                        help='decimal longitude for Safecast submissions (default: 0.0)')
    parser.add_argument('--safecast-height',
                        type=float,
                        default=0.0,
                        help='height (altitude) in meters for Safecast submissions (default: 0.0)')
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
                         args.datalog_datetime_start,
                         args.datalog_datetime_end,
                         args.datalog_max_record_num)

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
