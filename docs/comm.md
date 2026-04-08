# Rad Pro USB Communications Protocol

This document outlines the configuration and commands for establishing a USB data connection with a Rad Pro device using an ASCII-based, request-response protocol. All commands and responses terminate with `\r\n` (ASCII carriage return and newline).

## USB Configuration

To connect to a Rad Pro device via USB, use the following settings:

- **Baudrate**: 115200
- **Data bits**: 8
- **Parity**: None
- **Stop bits**: 1
- **Flow control**: None

## Protocol Overview

The protocol operates on a request-response model, with commands sent as ASCII text. Responses begin with `OK` for success or `ERROR` for invalid/erroneous requests. All numerical values are in decimal format unless otherwise specified.

## Commands

### Get Device Identification

- **Request**: `GET deviceId\r\n`
- **Response**: `OK [hardware-id];[software-id];[device-id]\r\n`
- **Description**: Retrieves the device's hardware and software details.
  - `[hardware-id]`: Hardware type, e.g., `FS2011 (STM32F051C8)`, `Bosean FS-600`, `GQ GMC-800`, or `Rad Pro simulator`.
  - `[software-id]`: Software version, e.g., `Rad Pro 2.0/en` (version/language).
  - `[device-id]`: Unique device identifier.
- **Example**:

  ```
  GET deviceId
  OK FS2011 (STM32F051C8);Rad Pro 2.0/en;b5706d937087f975b5812810
  ```

### Get Device Battery Voltage

- **Request**: `GET deviceBatteryVoltage\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the battery voltage (all cells) in volts, with three decimal places.
- **Example**:

  ```
  GET deviceBatteryVoltage
  OK 1.421
  ```

### GEt Device Date And Time

- **Request**: `GET deviceTime\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the device's current UNIX timestamp (seconds since 1/1/1970).
- **Example**:

  ```
  GET deviceTime
  OK 1690000000
  ```

### SEt Device Date And Time

- **Request**: `SET deviceTime [value]\r\n`
- **Response**: `OK\r\n`
- **Description**: Sets the device's date and time using a UNIX timestamp.
- **Example**:

  ```
  SET deviceTime 1690000300
  OK
  ```

### Get Device Time Zone

- **Request**: `GET deviceTimeZone\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the device's time zone offset from UTC (in hours).
- **Example**:

  ```
  GET deviceTimeZone
  OK 1.0
  ```

### Set Device Time Zone

- **Request**: `SET deviceTimeZone [value]\r\n`
- **Response**: `OK\r\n`
- **Description**: Sets the device's time zone offset from UTC (in hours).
- **Example**:

  ```
  SET deviceTimeZone -5.0
  OK
  ```

### Get Tube Lifetime

- **Request**: `GET tubeTime\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the tube's operational lifetime in seconds.
- **Example**:

  ```
  GET tubeTime
  OK 16000
  ```

### Set Tube Lifetime

- **Request**: `SET tubeTime [value]\r\n`
- **Response**: `OK\r\n`
- **Description**: Sets the tube's operational lifetime in seconds.
- **Example**:

  ```
  SET tubeTime 17000
  OK
  ```

### Get Tube Pulse Count

- **Request**: `GET tubePulseCount\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the tube's lifetime pulse count, which overflows to 0 after 2³²-1. Updated continuously.
- **Example**:

  ```
  GET tubePulseCount
  OK 1500
  ```

### Set Tube Pulse Count

- **Request**: `SET tubePulseCount [value]\r\n`
- **Response**: `OK\r\n`
- **Description**: Sets the tube's lifetime pulse count.
- **Example**:

  ```
  SET tubePulseCount 1600
  OK
  ```

### Get Tube Rate

- **Request**: `GET tubeRate\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the tube's instantaneous rate in counts per minute (cpm), with three decimal places, updated every second. For better precision, calculate the rate using two `GET tubePulseCount` measurements over a time interval: `(pulse_count_end - pulse_count_start) / time_interval_in_minutes`.
- **Example**:

  ```
  GET tubeRate
  OK 142.857
  ```

### Get Tube Sensitivity

- **Request**: `GET tubeSensitivity\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the tube's sensitivity in cpm/µSv/h, with three decimal places.
- **Example**:

  ```
  GET tubeSensitivity
  OK 153.800
  ```

### Get Tube Dead Time

- **Request**: `GET tubeDeadTime\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns an upper bound of the tube's dead time in seconds, with seven decimal places.
- **Example**:

  ```
  GET tubeDeadTime
  OK 0.0002420
  ```

### Get Tube Dead-Time Compensation

- **Request**: `GET tubeDeadTimeCompensation\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the tube's dead-time compensation in seconds, with seven decimal places. Returns `0.0000000` if disabled.
- **Example**:

  ```
  GET tubeDeadTimeCompensation
  OK 0.0002500
  ```

### Get Tube PWM Frequency (Supported Devices)

- **Request**: `GET tubeHVFrequency\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the PWM frequency of the high-voltage generator in Hz, with two decimal places.
- **Example**:

  ```
  GET tubeHVFrequency
  OK 1250.00
  ```

### Set Tube PWM Frequency (Supported Devices)

- **Request**: `SET tubeHVFrequency [value]\r\n`
- **Response**: `OK\r\n`
- **Description**: Sets the PWM frequency of the high-voltage generator (100 to 100000 Hz), with up to two decimal places.
- **Example**:

  ```
  SET tubeHVFrequency 2500.00
  OK
  ```

### Get tube PWM Duty Cycle (Supported Devices)

- **Request**: `GET tubeHVDutyCycle\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the PWM duty cycle of the high-voltage generator, with five decimal places.
- **Example**:

  ```
  GET tubeHVDutyCycle
  OK 0.09750
  ```

### Set Tube PWM Duty Cycle (Supported Devices)

- **Request**: `SET tubeHVDutyCycle [value]\r\n`
- **Response**: `OK\r\n`
- **Description**: Sets the PWM duty cycle (0.0 to 1.0), with up to five decimal places.
- **Example**:

  ```
  SET tubeHVDutyCycle 0.05
  OK
  ```

### Get Electric Field (Supported Devices)

- **Request**: `GET electricField\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the electric field in volts per meter, with three decimal places.
- **Example**:

  ```
  GET electricField
  OK 16.231
  ```

### Get Magnetic Field (Supported Devices)

- **Request**: `GET magneticField\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns the magnetic field in Tesla, with nine decimal places.
- **Example**:

  ```
  GET magneticField
  OK 0.000000025
  ```

### Retrieve Data Log

- **Request**: `GET datalog [start-time] [end-time] [max-record-num]\r\n`
- **Response**: `OK [data]\r\n`
- **Description**: Fetches data log records stored in flash memory. All parameters are optional.
  - `[start-time]`: UNIX timestamp (seconds since 1/1/1970) to include records from this time onward. Use `0` to include all earlier records.
  - `[end-time]`: UNIX timestamp to include records up to this time. Use `4294967295` to include all later records.
  - `[max-record-num]`: Maximum number of records to return.
  - `[data]`: Semicolon-separated records, each containing comma-separated fields. The first record lists field names (`time`, `tubePulseCount`), followed by measurements records (ordered from least to most recent). Empty records (`;;`) indicate the start of a new logging session.
  - Note: Data logging is paused during the download process.
- **Example**:

  ```
  GET datalog 1690000000
  OK time,tubePulseCount;;1690000000,1542;1690000060,1618;1690000120,1693
  ```

### Reset Data Log

- **Request**: `RESET datalog\r\n`
- **Response**: `OK\r\n`
- **Description**: Clears the data log in flash memory.
- **Example**:

  ```
  RESET datalog
  OK
  ```

### Retrieve Random Data

- **Request**: `GET randomData\r\n`
- **Response**: `OK [value]\r\n`
- **Description**: Returns up to 16 bytes from the random generator, in hexadecimal (0-9, a-f).
- **Example**:

  ```
  GET randomData
  OK 9155facb75c00e331cf7fd625102f37a
  ```

### Start Bootloader (Supported Devices)

- **Request**: `START bootloader\r\n`
- **Response**: `OK\r\n`
- **Description**: Initiates the system bootloader for device updates.
- **Example**:

  ```
  START bootloader
  OK
  ```

### Error Handling

- **Response**: `ERROR\r\n`
- **Description**: Returned for invalid or erroneous requests.
- **Example**:

  ```
  SET time
  ERROR
  ```
