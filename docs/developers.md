# Developers

## Communications

To connect to Rad Pro through a serial port or ST-LINK dongle through SWD, check out the [description of the communications protocol](comm.md).

## Build

To build the software, follow these steps:

* Install [Visual Studio Code](https://code.visualstudio.com/).
* From Visual Studio Code, install the [platform.io](https://platformio.org/) extension.
* Open the `platform.io` folder to begin building the firmware.
* Once you've built the firmware, sign the resulting binaries using Python and the `tools/radpro-sign.py` script.
* You can also build the software as a simulation by opening the project's root folder from Visual Studio Code. You'll need the [libsdl2](https://github.com/libsdl-org/SDL) and [libsercomm](https://github.com/ingeniamc/sercomm) library, which you can install using the [vcpkg](https://vcpkg.io/en/getting-started.html) package manager.
