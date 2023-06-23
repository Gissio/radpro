## Introduction

Rad Pro (formerly FS2011 Pro) is a custom firmware that enhances the capabilities of the several low-cost radiation meters/Geiger counters ([FS2011](https://www.amazon.com/s?k=fs2011), [FS9000](https://www.amazon.com/s?k=fs9000), [NR-950](https://www.amazon.com/s?k=nr950) and [YT-203B](https://www.amazon.com/s?k=yt-203b)). Support for the [Bosean FS-600](https://www.bosean.net/products/nuclear_radiation_detector.html) and [Bosean FS-1000](https://www.bosean.net/products/FS-1000_nuclear_radiation_detector.html) is planned.

Rad Pro can be used to:

* Measure instantaneous radiation rate, based on the last 11 pulses or a 5-second moving-average filter.
* Measure average radiation rate over a time period.
* Measure cumulative dose (stored in non-volatile memory).
* Generate customizable rate and dose alarms.
* Plot historical radiation rate over different time periods (2 minutes, 10 minutes, 1 hour, 6 hours and 24 hours).
* Generate secure passwords (letters & numbers, full ASCII), random hexadecimal and decimal numbers, coin flips and dice throws (6-sided, 4-sided, 8-sided, 12-sided and 20-sided).
* Log the measurements on a computer, optionally sending the data to the https://radmon.org radiation monitoring website.

Watch a demonstration of Rad Pro in action:

[![Rad Pro Demonstration](docs/img/radpro-video.jpg)](https://www.youtube.com/watch?v=7dpVG1jSLn8)

## Features

Rad Pro features:

* An friendly user interface for easy operation.
* Multiple measurement units: Sievert, rem, counts per minute (cpm) and counts per second (cps).
* 95% confidence intervals for accurate readings.
* Measurement hold for instantaneous rate, average rate, and dose.
* Overflow and overload alerts.
* Configurable pulse click sounds: off, quiet, loud.
* Configurable backlight: off, on for 10 seconds, on for 60 seconds, pulse flashes or always on.
* Configurable battery type selection for accurate battery level monitoring.
* Support for multiple Geiger-Müller tubes: HH614 (48 mm long), M4011 (105 mm long), SBM-20 (108 mm long), SI-3BG (50 mm long).
* Life statistics to track usage and performance of the device.
* Up to 40% more battery life compared to the original firmware.
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Legal

Before installing Rad Pro, please read the following important information:

__Disclaimer:__ The authors of this software are not responsible or liable for any claims, damages, losses, expenses, costs, or liabilities whatsoever resulting or arising directly or indirectly from the use of this software.

__Notice:__ Please be aware that installing this software will void the warranty of your device.

By proceeding with the installation, you acknowledge that you have read and understood these terms and accept full responsibility for any consequences that may result from using this software. If you do not agree with these terms, do not install this software.

## Installation on the FS2011/FS9000/NR-950/YT-203B

To install Rad Pro on your FS2011/FS9000/NR-950/YT-203B radiation meter/Geiger counter, you will need the following tools and components:

* An ST-Link V2 USB dongle (or clone)
* A soldering iron and solder
* A 4-pin header
* A Philips screwdriver
* [stlink](https://github.com/stlink-org/stlink/releases)

Follow these steps to install Rad Pro on your device:

* Remove the battery cover and batteries.
* Remove the four screws holding the back case.
* Remove the four screws holding the electronics board on the front case.
* Remove the electronics board and ensure that it matches the board shown in the following picture (the Geiger-Müller tube may be different or mounted differently):

![FS2011 circuit board](docs/img/fs2011-board.jpg)

If your board looks different, you may have a different hardware revision. In such cases, this firmware may not function properly. Please post an issue, including a photograph of your board (both sides) and a description of the problem.

* Solder the 4-pin header to XS1 on the board.
* Connect the ST-Link V2 device to XS1. The pins, from top to bottom, are:
  * GND
  * SWCLK
  * SWDIO
  * +3.3V
* Before installing for the first time, back up the original firmware. If you skip this step, you won't be able to restore your device if something goes wrong. To create a backup, open a terminal and go to the `bin` folder within the stlink folder. Then, execute the following command and verify that the resulting file, `backup.bin`, has a size of 65536 bytes:

      st-flash read backup.bin 0x08000000 0x10000

* Download the latest firmware from the [releases](https://github.com/Gissio/radpro/releases): get `radpro-fs2011-x.y.z-install.bin` when installing for the first time or `radpro-fs2011-x.y.z-update.bin` when updating.
* Open a terminal, go to the `bin` folder within the stlink folder, and run this command:

      st-flash write [firmware-filename] 0x08000000

### Optional mods

* For easier access to XS1, use a file to extend the hole for the battery holder clip so that a 4-pin header connector can fit. Solder four cables approximately 15 cm long between XS1 and a 4-pin header. Using a glue gun, attach the header to the inside of the back case, so you can access the header with the battery case open.
* For improved measurement, align the Geiger-Müller tube to the holes of the back case using a heat gun/glue gun if the tube is mounted vertically. Be careful, as the tube's glass is very delicate.
* Some AA rechargeable batteries have low-profile caps that don't make electrical contact with the battery holder. To fix this problem, apply solder on the battery holder's pads.
* To increase the buzzer's volume, drill a hole on the back case in front of the buzzer.
* The device is able to charge Ni-MH batteries if you add a female USB port and connect its GND and +5V port to the 0V and 5V pad of the electronics board.

## Installation on other devices

Currently, Rad Pro supports the FS2011, FS9000, NR-950 and YT-203B. If you would like to contribute to this project and add support for other devices, please contact me.

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

* Install [Visual Studio Code](https://code.visualstudio.com/).
* From Visual Studio Code, install the [platform.io](https://platformio.org/) extension.
* Open the `platform.io` folder to begin building the firmware.
* Once you've built the firmware, sign the resulting binary using Python and the `tools/sign-firmware.py` script.
* You can also build the software as a simulator by opening the project's root folder from Visual Studio Code. You'll need the `sdl2` library, which you can install using the [vcpkg](https://vcpkg.io/en/getting-started.html) package manager.

## Thanks

* Special thanks to the u8g2 team.
* Special thanks to Alfred90 for device testing.
