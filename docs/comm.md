# Communications protocol

To establish a connection with a Rad Pro device, you can use either a serial port or an SWD connection. When using the serial port, make sure to configure it as follows:

* Baudrate: 115200
* Data bits: 8
* Parity: None
* Stop bits: 1
* Flow control: None

The communications protocol is ASCII-based and follows a request-response model.

In the following description, `\n` represents the newline character.

## Requests

### Get device identification

* Request: `GET deviceId\n`
* Response: `OK [hardware-id];[software-id];[device-id]\n`

Identifies the device's hardware and software. The response consists of these components:

* `[hardware-id]`: The device's hardware, which may be one of the following: `FS2011 (STM32F051C8)`, `FS2011 (GD32F150C8)`, `FS2011 (GD32F103C8)`, `Bosean FS-600`, `Bosean FS-1000`, `FNIRSI GC-01 (CH32F103C8)`, `FNIRSI GC-01 (APM32F103CB)` or `Rad Pro simulator`.
* `[software-id]`: The device's software, typically `Rad Pro` followed by the software version.
* `[device-id]`: A unique device ID.

Example:

    GET deviceId
    OK FS2011 (STM32F051C8);Rad Pro 2.0;9748af1b

### Get device battery voltage

* Request: `GET deviceBatteryVoltage\n`
* Response: `OK [value]\n`

Returns the device's battery voltage (averaged per cell) in Volt, provided in decimal format with three decimal places.

Example:

    GET deviceBatteryVoltage
    OK 1.421

### Get device date and time

* Request: `GET deviceTime\n`
* Response: `OK [value]\n`

Returns the UNIX timestamp (number of seconds since 1/1/1970) of the device's current date and time, provided in decimal format.

Example:

    GET deviceTime
    OK 1690000000

### Set device date and time

* Request: `SET deviceTime [value]\n`
* Response: `OK\n`

Sets the device's current date and time as a UNIX timestamp (number of seconds since 1/1/1970), provided in decimal format.

Example:

    SET deviceTime 1690000000
    OK

### Get tube's life time

* Request: `GET tubeTime\n`
* Response: `OK [value]\n`

Returns the tube's life time in seconds, provided in decimal format.

Example:

    GET tubeTime
    OK 16000

### Set tube's life time

* Request: `SET tubeTime [value]\n`
* Response: `OK\n`

Sets the tube's life time in seconds, provided in decimal format.

Example:

    SET tubeTime 16000
    OK

### Get tube's life pulse count

* Request: `GET tubePulseCount\n`
* Response: `OK [value]\n`

Returns the tube's life pulse count, provided in decimal format. After (2^32 - 1), the value overflows to 0.

Example:

    GET tubePulseCount
    OK 1500

### Set tube's life pulse count

* Request: `SET tubePulseCount [value]\n`
* Response: `OK\n`

Sets the tube's life pulse count, provided in decimal format.

Example:

    SET tubePulseCount 1500
    OK

### Get tube rate value

* Request: `GET tubeRate\n`
* Response: `OK [value]\n`

Returns the tube's instantaneous rate measured in counts per minute (cpm), provided in decimal format with three decimal places.

Example:

    GET tubeRate
    OK 142.857

For improved accuracy, perform two `GET tubePulseCount` measurements over a time period and divide the pulse count difference by the time period (in minutes).

### Get tube conversion factor

* Request: `GET tubeConversionFactor\n`
* Response: `OK [value]\n`

Returns the tube's conversion factor in cpm/µSv/h, provided in decimal format with three decimal places.

Example:

    GET tubeConversionFactor
    OK 153.800

### Get tube dead-time measurement

* Request: `GET tubeDeadTime\n`
* Response: `OK [value]\n`

Returns a measured upper bound of the tube's dead time in seconds, provided in decimal format with seven decimal places.

Example:

    GET tubeDeadTime
    OK 0.0002425

### Get tube dead-time compensation

* Request: `GET tubeDeadTimeCompensation\n`
* Response: `OK [value]\n`

Returns the tube's dead-time compensation in seconds, provided in decimal format with seven decimal places. Returns `0.0000000` if dead-time compensation is turned off.

Example:

    GET tubeDeadTimeCompensation
    OK 0.0002500

### Get tube background compensation

* Request: `GET tubeBackgroundCompensation\n`
* Response: `OK [value]\n`

Returns the tube's background compensation in cpm, provided in decimal format with one decimal place. Returns `0.0` if background compensation is turned off.

Example:

    GET tubeBackgroundCompensation
    OK 2.0

### Get tube PWM frequency

* Request: `GET tubeHVFrequency\n` 
* Response: `OK [value]\n`

Returns the tube's PWM frequency of the high voltage generator in Hz, provided in decimal format with two decimal places.

Example:

    GET tubeHVFrequency
    OK 1250.00

### Get tube PWM duty cycle

* Request: `GET tubeHVDutyCycle\n` 
* Response: `OK [value]\n`

Returns the tube's PWM duty-cycle of the high voltage generator, provided in decimal format with four decimal places.

Example:

    GET tubeHVDutyCycle
    OK 0.0975

### Get data log

* Request: `GET datalog [time]\n`
* Response: `OK [data]\n`

Returns the data log stored in the device's flash memory. `[time]` is an optional UNIX timestamp (number of seconds since 1/1/1970), provided in decimal format, that limits the output to entries that occur at this date and time or later. `[data]` is a list of records, separated by `;`. Each record's fields are separated by `,`. The first record lists the field names, and the following records are actual measurements ordered from least recent to most recent. Valid fields include:

* `time`: The record's UNIX timestamp (number of seconds since 1/1/1970), provided in decimal format.
* `tubePulseCount`: The tube's life pulse count, provided in decimal format.

Example:

    GET datalog 1690000000
    OK time,tubePulseCount;1690000000,1542;1690000060,1618;1690000120,1693

### Get random data from the random generator

* Request: `GET randomData\n`
* Response: `OK [value]\n`

Returns up to 16 bytes from the random generator, provided as hexadecimal digits (0-9, a-f).

Example:

    GET randomData
    OK 9155facb75c00e331cf7fd625102f37a

### Invalid or erroneous requests

Invalid or erroneous requests yield an `ERROR\n` response.

Example:

    SET time
    ERROR
