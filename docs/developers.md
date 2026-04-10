# Rad Pro Developers's Manual

## Build

To build the software, follow these steps:

* Install [Visual Studio Code](https://code.visualstudio.com/).
* From Visual Studio Code / Extensions, install the [PlatformIO IDE](https://platformio.org/) extension.
* Open the `platform.io` folder to begin building the firmware.
* Once you've built the firmware, sign the resulting binaries with the `tools/sign.py` script: from a terminal, install the [requirements](reference-manual.md#radpro-tool), go to the `tools` folder and start the `sign.py` script. The signed `.bin` firmware files should appear in the `tools` folder.
* You can also build the software as a simulator by opening the project's root folder from Visual Studio Code. You'll need the [libsdl2](https://github.com/libsdl-org/SDL) and [libsercomm](https://github.com/ingeniamc/sercomm) library, which you can install using the [vcpkg](https://vcpkg.io/en/getting-started.html) package manager.

## Internal Storage Format

Data is stored internally using a compressed storage format. Integer values are represented in big-endian byte order.

### Data Encodings

    0b0xxxxxxx

Encodes a differential value from 0 to 127.

    0b10xxxxxx 0bxxxxxxxx

Encodes a differential value from 0 to 16383.

    0b110xxxxx 0bxxxxxxxx 0bxxxxxxxx

Encodes a differential value from 0 to 2097151.

    0b1110xxxx 0bxxxxxxxx 0bxxxxxxxx 0bxxxxxxxx

Encodes a differential value from 0 to 268435455.

    0b11110000 0bxxxxxxxx 0bxxxxxxxx 0bxxxxxxxx 0bxxxxxxxx

Encodes a differential value from 0 to 4294967295.

    0b11110001 [32-bit timestamp] [32-bit pulse count]

Sets 60 minute time intervals and encodes initial timestamp and pulse count.

    0b11110010 [32-bit timestamp] [32-bit pulse count]

Sets 10 minute time intervals and encodes initial timestamp and pulse count.

    0b11110011 [32-bit timestamp] [32-bit pulse count]

Sets 1 minute time intervals and encodes initial timestamp and pulse count.

    0b11110100 [32-bit timestamp] [32-bit pulse count]

Sets 10 second time intervals and encodes initial timestamp and pulse count.

    0b11110101 [32-bit timestamp] [32-bit pulse count]

Sets 1 second time intervals and encodes initial timestamp and pulse count.

    0b11111000

Marks the start of a new logging session.

    0b11111110

Dummy value for memory alignment.
