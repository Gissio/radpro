# Communications protocol

To establish a USB data connection with a Rad Pro device, make sure to configure it as follows:

* Baudrate: 115200
* Data bits: 8
* Parity: None
* Stop bits: 1
* Flow control: None

The communications protocol is ASCII-based and follows a request-response model.

In the following description, `\r\n` represents the ASCII return and the newline character sequence.

## Requests

### Get device identification

* Request: `GET deviceId\r\n`
* Response: `OK [hardware-id];[software-id];[device-id]\r\n`

Identifies the device's hardware and software. The response consists of these components:

* `[hardware-id]`: The device's hardware, which may be one of the following: `FS2011 (STM32F051C8)`, `FS2011 (GD32F150C8)`, `FS2011 (GD32F103C8)`, `Bosean FS-600`, `Bosean FS-1000`, `FNIRSI GC-01 (CH32F103C8)`, `FNIRSI GC-01 (APM32F103CB)` or `Rad Pro simulator`.
* `[software-id]`: The device's software, typically `Rad Pro` followed by the software version.
* `[device-id]`: A unique device ID.

Example:

    GET deviceId
    OK FS2011 (STM32F051C8);Rad Pro 2.0;b5706d937087f975b5812810

### Get device battery voltage

* Request: `GET deviceBatteryVoltage\r\n`
* Response: `OK [value]\r\n`

Returns the device's battery voltage (averaged per cell) in Volt, provided in decimal format with three decimal places.

Example:

    GET deviceBatteryVoltage
    OK 1.421

### Get device date and time

* Request: `GET deviceTime\r\n`
* Response: `OK [value]\r\n`

Returns the UNIX timestamp (number of seconds since 1/1/1970) of the device's current date and time, provided in decimal format.

Example:

    GET deviceTime
    OK 1690000000

### Set device date and time

* Request: `SET deviceTime [value]\r\n`
* Response: `OK\r\n`

Sets the device's current date and time as a UNIX timestamp (number of seconds since 1/1/1970), provided in decimal format.

Example:

    SET deviceTime 1690000300
    OK

### Get device time zone

* Request: `GET deviceTimeZone\r\n`
* Response: `OK [value]\r\n`

Returns the device's time zone (in hours relative to UTC).

Example:

    GET deviceTimeZone
    OK 1.0

### Set device time zone

* Request: `SET deviceTimeZone [value]\r\n`
* Response: `OK\r\n`

Sets the device's time zone (in hours relative to UTC).

Example:

    SET deviceTimeZone -5.0
    OK

### Get tube's life time

* Request: `GET tubeTime\r\n`
* Response: `OK [value]\r\n`

Returns the tube's life time in seconds, provided in decimal format.

Example:

    GET tubeTime
    OK 16000

### Set tube's life time

* Request: `SET tubeTime [value]\r\n`
* Response: `OK\r\n`

Sets the tube's life time in seconds, provided in decimal format.

Example:

    SET tubeTime 17000
    OK

### Get tube's life pulse count

* Request: `GET tubePulseCount\r\n`
* Response: `OK [value]\r\n`

Returns the tube's life pulse count, provided in decimal format. After (2^32 - 1), the value overflows to 0. The value is updated continuously.

Example:

    GET tubePulseCount
    OK 1500

### Set tube's life pulse count

* Request: `SET tubePulseCount [value]\r\n`
* Response: `OK\r\n`

Sets the tube's life pulse count, provided in decimal format.

Example:

    SET tubePulseCount 1600
    OK

### Get tube rate value

* Request: `GET tubeRate\r\n`
* Response: `OK [value]\r\n`

Returns the tube's instantaneous rate measured in counts per minute (cpm), provided in decimal format with three decimal places. The value is updated once per second.

Example:

    GET tubeRate
    OK 142.857

For improved precision, perform two `GET tubePulseCount` measurements over a time period and divide the pulse count difference by the time period (in minutes).

### Get tube sensitivity

* Request: `GET tubeSensitivity\r\n`
* Response: `OK [value]\r\n`

Returns the tube's sensitivity in cpm/µSv/h, provided in decimal format with three decimal places.

Example:

    GET tubeSensitivity
    OK 153.800

### Get tube dead-time measurement

* Request: `GET tubeDeadTime\r\n`
* Response: `OK [value]\r\n`

Returns a measured upper bound of the tube's dead time in seconds, provided in decimal format with seven decimal places.

Example:

    GET tubeDeadTime
    OK 0.0002425

### Get tube dead-time compensation

* Request: `GET tubeDeadTimeCompensation\r\n`
* Response: `OK [value]\r\n`

Returns the tube's dead-time compensation in seconds, provided in decimal format with seven decimal places. Returns `0.0000000` if dead-time compensation is turned off.

Example:

    GET tubeDeadTimeCompensation
    OK 0.0002500

### Get tube PWM frequency

(on supported devices)

* Request: `GET tubeHVFrequency\r\n`
* Response: `OK [value]\r\n`

Returns the tube's PWM frequency of the high voltage generator in Hz, provided in decimal format with two decimal places.

Example:

    GET tubeHVFrequency
    OK 1250.00

### Set tube PWM frequency

(on supported devices)

* Request: `SET tubeHVFrequency [value]\r\n`
* Response: `OK\r\n`

Enables the custom HV profile and sets its PWM frequency, provided in decimal format with up to two decimal places. Valid values are from 100 to 100000.

Example:

    SET tubeHVFrequency 2500.00
    OK

### Get tube PWM duty cycle

(on supported devices)

* Request: `GET tubeHVDutyCycle\r\n`
* Response: `OK [value]\r\n`

Returns the tube's PWM duty-cycle of the high voltage generator, provided in decimal format with five decimal places.

Example:

    GET tubeHVDutyCycle
    OK 0.09750

### Set tube PWM duty cycle

(on supported devices)

* Request: `SET tubeHVDutyCycle [value]\r\n`
* Response: `OK\r\n`

Enables the custom HV profile and sets its PWM duty-cycle, provided in decimal format with up to five decimal places. Valid values are 0.0 to 1.0.

Example:

    SET tubeHVDutyCycle 0.05
    OK

### Get data log

* Request: `GET datalog [time]\r\n`
* Response: `OK [data]\r\n`

Returns the data log stored in the device's flash memory. `[time]` is an optional UNIX timestamp (number of seconds since 1/1/1970), provided in decimal format, that limits the output to entries that occur at this date and time or later. `[data]` is a list of records, separated by `;`. Each record's fields are separated by `,`. The first record lists the field names, and the following records are actual measurements ordered from least recent to most recent. Valid fields include:

* `time`: The record's UNIX timestamp (number of seconds since 1/1/1970), provided in decimal format.
* `tubePulseCount`: The tube's life pulse count, provided in decimal format.

During data log download, data is not logged.

Example:

    GET datalog 1690000000
    OK time,tubePulseCount;1690000000,1542;1690000060,1618;1690000120,1693

### Reset data log

* Request: `RESET datalog\r\n`
* Response: `OK\r\n`

Resets the data log stored in the device's flash memory.

Example:

    RESET datalog
    OK

### Get random data from the random generator

* Request: `GET randomData\r\n`
* Response: `OK [value]\r\n`

Returns up to 16 bytes from the random generator, each byte provided as a pair of hexadecimal digits (0-9, a-f).

Example:

    GET randomData
    OK 9155facb75c00e331cf7fd625102f37a

### Start bootloader

* Request: `START bootloader\r\n`
* Response: `OK\r\n`

Starts the system bootloader for updating the device.

Example:

    START bootloader
    OK

### Invalid or erroneous requests

Invalid or erroneous requests yield an `ERROR\r\n` response.

Example:

    SET time
    ERROR
