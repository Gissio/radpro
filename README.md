## Overview

FS2011 Pro is an advanced firmware that enhances the capabilities of the FS2011/FS2011+ radiation detector/Geiger counter.

Watch a demonstration of FS2011 Pro in action:

[![FS2011 Pro Demonstration](docs/img/fs2011pro-video.jpg)](https://www.youtube.com/watch?v=7dpVG1jSLn8)

## Features

* Instantaneous rate, average rate, dose, and history modes.
* True random number generator to generate secure passwords (letters & numbres, full ASCII), random hexadecimal and decimal numbers, coin flips and dice throws (6-sided, 4-sided, 8-sided, 12-sided and 20-sided).
* Intuitive user interface for easy navigation and hassle-free use.
* Measurements in Sievert, rem, cpm and cps.
* Multiple history periods, including 2-minute, 10-minute, 1-hour, 6-hour and 24-hour options.
* 95% confidence intervals to ensure accurate readings.
* Measurement hold for instantaneous rate, average rate, and dose.
* Overload alert to prevent damage to the device.
* Customizable rate and dose alarms.
* Unique and fun feature: Nuclear chess.
* Configurable pulse click sounds: off, quiet, loud.
* Configurable backlight: off, pulse flashes, on for 10 seconds, on for 60 seconds, or always on.
* Configurable battery type for battery level monitoring.
* Multiple Geiger-Müller tubes: HH614 (48 mm long), M4011 (105 mm long), SBM-20 (108 mm long), SI-3BG (50 mm long).
* Device life statistics for monitoring usage and performance of the device.
* 40% more battery life compared to the original firmware.
* Power-on self-test and safety watchdog.

## Installation

Before installing FS2011 Pro, please read the following important information:

__Disclaimer:__ The authors of this software are not responsible or liable for any claims, damages, losses, expenses, costs, or liabilities whatsoever resulting or arising directly or indirectly from the use of this software.

__Notice:__ Please be aware that installing this software will void the warranty of your device.

By proceeding with the installation, you acknowledge that you have read and understood these terms and accept full responsibility for any consequences that may result from using this software. If you do not agree with these terms, do not install this software.

### Requirements

To install FS2011 Pro, you will need the following tools and components:

* A Philips screwdriver
* A soldering iron and solder
* A 4-pin header
* An ST-Link V2 USB dongle (or clone)
* [stlink][stlink-link]

### Instructions

Follow these steps to install FS2011 Pro firmware on your FS2011/FS2011+ radiation detector/Geiger counter:

* Remove the battery cover and batteries.
* Unscrew and remove the four screws holding the back case.
* Remove the four screws holding the electronics board on the front case.
* Remove the electronics board and ensure that it matches the board shown in the following picture (the Geiger-Müller tube may be different or mounted differently):

![FS2011 circuit board](docs/img/fs2011-board.jpg)

If your board looks different, you may have a different hardware version and this firmware may not work correctly.

* Solder the 4-pin header to XS1 on the board.
* Optionally, align the Geiger-Müller tube to the holes of the back case using a heat gun/glue gun if the tube is mounted vertically. Be careful, as the tube's glass is very delicate.
* Optionally, apply solder on the battery holder's pads so that low-profile AA batteries make good electrical contact.
* Optionally, increase the volume by drilling a hole on the back case in front of the buzzer.
* Optionally, add a female USB port and connect GND and +5V to the 0V and 5V pads of the electronics board. This modification will allow your FS2011 to run continuously and even charge Ni-MH batteries.
* Connect the ST-Link V2 device to XS1. The pins, from top to bottom, are:
  * GND
  * SWCLK
  * SWDIO
  * +3V3
* Before installing FS2011 Pro for the first time, back up the original firmware. If you skip this step, you won't be able to restore your device if something goes wrong. To create a backup, open a terminal and go to the `bin` folder within the stlink folder. Then, execute the following command and verify that the resulting file, `backup.bin`, has a size of 65536 bytes:

  ```
  st-flash read backup.bin 0x08000000 0x10000
  ```
* Download the latest FS2011 Pro firmware from the [releases][releases-link]: use the "-install" version when installing for the first time and the "-update" version when updating.
* To install FS2011 Pro, open a terminal, go to the `bin` folder within the stlink folder, and run this command:

  ```
  st-flash write [Firmware filename] 0x08000000
  ```

## Usage

To use this firmware, follow these instructions:

* To turn the device on and off, long-press the POWER key.
* To turn the backlight on (if the backlight is enabled), short-press the POWER key.

* To change the mode/selection, press the UP and DOWN keys.

* To hold/unhold the current measurement (in instantaneous rate, average rate, and dose mode), short-press the PLAY/PAUSE key.
* To reset the mode's measurements, long-press the PLAY/PAUSE key.

* To enter the menu/select a menu option, press the MENU/OK key. To go back from a menu/menu option, press the PLAY/PAUSE key.

* The random number generator produces 16 symbols at a time. If you need more symbols, simply return to the menu and initiate a new run.

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

## Building

To build the software, follow these steps:

* Install [Visual Studio Code][vscode-link].
* From Visual Studio Code, install the [platform.io][platformio-link] extension.
* Open the `platform.io` folder to begin building the firmware.
* Once you've built the firmware, sign the resulting binary using Python and the `tools/sign.py` script.
* You can also build the software as a simulator by opening the project's root folder from Visual Studio Code. You'll need the `sdl2` library, which you can install using the [vcpkg][vcpkg-link] package manager.

## Thanks

* Special thanks to the u8g2 team.
* Special thanks to Alfred90 for device testing.

[stlink-link]: https://github.com/stlink-org/stlink/releases
[releases-link]: https://github.com/Gissio/fs2011pro/releases
[vscode-link]: https://code.visualstudio.com/
[platformio-link]: https://platform.io/
[vcpkg-link]: https://vcpkg.io/en/getting-started.html
