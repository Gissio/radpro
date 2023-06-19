## Overview

Rad Pro (formerly FS2011 Pro) is an open-source firmware that enhances the capabilities of portable radiation detectors/Geiger counters. Currently the [FS2011](https://www.amazon.com/s?k=fs2011) is supported. Updates to support the [Bosean FS-600](https://www.bosean.net/products/nuclear_radiation_detector.html) and [Bosean FS-1000](https://www.bosean.net/products/FS-1000_nuclear_radiation_detector.html) are planned.

Watch a demonstration of Rad Pro in action:

[![Rad Pro Demonstration](docs/img/radpro-video.jpg)](https://www.youtube.com/watch?v=7dpVG1jSLn8)

## Features

* Intuitive user interface for easy navigation and hassle-free experience.
* Instantaneous rate, average rate, dose, and history modes.
* Measurements in Sievert, rem, cpm and cps.
* Customizable rate and dose alarms.
* Measurement hold for instantaneous rate, average rate, and dose.
* Storage of radiation dose in non-volatile memory.
* Multiple history periods, including 2-minute, 10-minute, 1-hour, 6-hour and 24-hour options.
* Data logging.
* Client for the https://radmon.org radiation monitoring website.
* 95% confidence intervals to ensure accurate readings.
* Overload alert.
* Configurable pulse click sounds: off, quiet, loud.
* Configurable backlight: off, pulse flashes, on for 10 seconds, on for 60 seconds, or always on.
* Configurable battery type for battery level monitoring.
* Support for multiple Geiger-Müller tubes: HH614 (48 mm long), M4011 (105 mm long), SBM-20 (108 mm long), SI-3BG (50 mm long).
* Device statistics for monitoring usage and performance of the device.
* True random number generator for generating secure passwords (letters & numbres, full ASCII), random hexadecimal and decimal numbers, coin flips and dice throws (6-sided, 4-sided, 8-sided, 12-sided and 20-sided).
* Up to 40% more battery life compared to the original firmware.
* Power-on self-test and safety watchdog.
* Unique and fun feature: Nuclear chess.

## Installation

Before installing Rad Pro, please read the following important information:

__Disclaimer:__ The authors of this software are not responsible or liable for any claims, damages, losses, expenses, costs, or liabilities whatsoever resulting or arising directly or indirectly from the use of this software.

__Notice:__ Please be aware that installing this software will void the warranty of your device.

By proceeding with the installation, you acknowledge that you have read and understood these terms and accept full responsibility for any consequences that may result from using this software. If you do not agree with these terms, do not install this software.

### Requirements

To install Rad Pro, you will need the following tools and components:

* A Philips screwdriver
* A soldering iron and solder
* A 4-pin header
* An ST-Link V2 USB dongle (or clone)
* [stlink][stlink-link]

### Instructions for FS2011 devices

Follow these steps to install Rad Pro firmware on your FS2011 radiation detector/Geiger counter:

* Remove the battery cover and batteries.
* Unscrew and remove the four screws holding the back case.
* Remove the four screws holding the electronics board on the front case.
* Remove the electronics board and ensure that it matches the board shown in the following picture (the Geiger-Müller tube may be different or mounted differently):

![FS2011 circuit board](docs/img/fs2011-board.jpg)

If your board looks different, you may have a different hardware version. In such cases, this firmware may not function properly, especially if your board does not have an STM32 microprocessor. Please post an issue, including a photograph of your board (both sides).

* Solder the 4-pin header to XS1 on the board.
* Optionally, align the Geiger-Müller tube to the holes of the back case using a heat gun/glue gun if the tube is mounted vertically. Be careful, as the tube's glass is very delicate.
* Optionally, apply solder on the battery holder's pads so that low-profile AA batteries make good electrical contact.
* Optionally, increase the volume by drilling a hole on the back case in front of the buzzer.
* Optionally, add a female USB port and connect GND and +5V to the 0V and 5V pads of the electronics board. This modification will allow your FS2011 to run continuously and even charge Ni-MH batteries.
* Connect the ST-Link V2 device to XS1. The pins, from top to bottom, are:
  * GND
  * SWCLK
  * SWDIO
  * +3.3V
* Before installing for the first time, back up the original firmware. If you skip this step, you won't be able to restore your device if something goes wrong. To create a backup, open a terminal and go to the `bin` folder within the stlink folder. Then, execute the following command and verify that the resulting file, `backup.bin`, has a size of 65536 bytes:

  ```
  st-flash read backup.bin 0x08000000 0x10000
  ```

* Download the latest firmware from the [releases][releases-link]: get `radpro-fs2011-x.y.z-install.bin` when installing for the first time or `radpro-fs2011-x.y.z-update.bin` when updating.
* Open a terminal, go to the `bin` folder within the stlink folder, and run this command:

  ```
  st-flash write [Firmware filename] 0x08000000
  ```

### Instructions for other devices

Currently, Rad Pro only supports the FS2011 radiation meter. If you would like to contribute to this project to add support other devices, please contact me.

## Usage

To use this firmware, follow these instructions:

* To power the device on and off, simply long-press the POWER key.
* To activate the backlight (if enabled), briefly press the POWER key.

* Use the UP and DOWN keys to change the mode or make selections.

* Briefly press the PLAY/PAUSE key to toggle between holding and unholding the current measurement in instantaneous rate, average rate, and dose mode.
* To reset the measurements for the selected mode, long-press the PLAY/PAUSE key.

* Press the MENU/OK key to access the menu and select various options.
* To go back from a menu or menu option, press the PLAY/PAUSE key.

* The device automatically stores the dose to non-volatile memory every hour. To ensure accurate dose storage, always power off the device by long-pressing the POWER key. Avoid removing batteries to power off.

* The random number generator generates 16 symbols per run. If additional symbols are needed, simply return to the menu and initiate a new run.

## Measurements

The software measures the radiation in the following modes:

### Instantaneous rate

The instantaneous rate is calculated as the average between a first pulse and the most recent pulse. If there are more than 11 pulses in 5 seconds, the first pulse is the first one to occur within the 5-second window. Otherwise, it is the first of the most recent 11 pulses.

The 95% confidence intervals assume a constant level of radiation over the averaging period.

### Average rate

The average rate is calculated as the pulse average between the first and last pulse in the time window.

The 95% confidence intervals assume a constant level of radiation over the averaging period.

### Dose

The dose is calculated from the number of pulses in the time window.

### History

The history is calculated from the instantaneous rate, sampled once per second.

### Random number generator

The random number generator produces one bit for every two new pulses by comparing the interval between them with a precision of 125 ns. The bit is discarded if the intervals are the same length. To prevent bias, every second bit is flipped. The generator stores entropy in a 512-bit buffer.

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Letters & numbers" consumes approximately 6 bits per symbol, "Full ASCII" consumes 7 bits, "20-sided dice" and "12-sided dice" consumes 5 bits, "Hexadecimal", "Decimal" consumes 4 bits, "8-sided dice" and "6-sided dice" consumes 3 bits, "4-sided dice" consumes 2 bits, and "Coin flip" consumes 1 bit.

To generate bits more quickly, use a radioactive source.

## Building the firmware

To build the software, follow these steps:

* Install [Visual Studio Code][vscode-link].
* From Visual Studio Code, install the [platform.io][platformio-link] extension.
* Open the `platform.io` folder to begin building the firmware.
* Once you've built the firmware, sign the resulting binary using Python and the `tools/sign-firmware.py` script.
* You can also build the software as a simulator by opening the project's root folder from Visual Studio Code. You'll need the `sdl2` library, which you can install using the [vcpkg][vcpkg-link] package manager.

## Thanks

* Special thanks to the u8g2 team.
* Special thanks to Alfred90 for device testing.

[stlink-link]: https://github.com/stlink-org/stlink/releases
[releases-link]: https://github.com/Gissio/fs2011pro/releases
[vscode-link]: https://code.visualstudio.com/
[platformio-link]: https://platform.io/
[vcpkg-link]: https://vcpkg.io/en/getting-started.html
