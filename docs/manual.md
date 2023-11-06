# Rad Pro User's manual

## Features

* Multiple measurement units: Sievert, rem, counts per minute (cpm) and counts per second (cps).
* Configurable average timer.
* Overflow alerts.
* Configurable pulse click sounds: off, quiet, loud.
* Configurable display options: backlight (on monochrome displays) or theme, brightness level and sleep (on color displays).
* Configurable battery type selection (on devices with a removable battery).
* Real-time clock data logging.
* Customizable Geiger-Müller settings: conversion factor, dead-time compensation and HV duty cycle for tube high-voltage control.
* Dead-time measurement.
* Life statistics for tracking of device usage.
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Keyboard mappings for different devices

### FS2011

  * Power on/off: Long-press POWER key.
  * Go up/down: UP and DOWN keys.
  * Enter menu/select option: MENU/OK key
  * Go back/hold measurement: PLAY/PAUSE key.
  * Reset measurement: Long-press PLAY/PAUSE key.

<!-- ### Bosean FS-600

  * Power on/off: Long-press POWER key.
  * Go up/down: UP and DOWN keys.
  * Enter menu/select option: RIGHT/CONFIG key
  * Go back/hold measurement: BACK key.
  * Reset measurement: Long-press BACK key.

### Bosean FS-1000

  * Power on/off: Extended press of the SET/POWER key.
  * Go up: MODE key.
  * Go down: SET/POWER key.
  * Enter menu/select option: Long-press SET/POWER key.
  * Go back/hold measurement: Long-press MODE key.
  * Reset measurement: Long-press both MODE and SET/POWER keys. -->

<!-- ### FNIRSI GC-01

  * Power on/off: Long-press OK/POWER key.
  * Go up/down: UP and DOWN keys.
  * Go down: Short-press SET/POWER key.
  * Enter menu/select option: RIGHT/CONFIG key.
  * Go back/hold measurement: LEFT/BACK key.
  * Reset measurement: Long-press LEFT/BACK keys. -->

## Measurements

For correct storage of the dose, always power off the device using the keyboard. Avoid removing batteries to power off.

The software measures the radiation in the following modes:

### Instantaneous rate

The instantaneous rate is calculated as the average between a first pulse and the most recent pulse. If there are more than 11 pulses in 5 seconds, the first pulse is the first one to occur within the 5-second window. Otherwise, it is the first of the most recent 11 pulses.

The 95% confidence intervals assume a constant level of radiation over the averaging period.

### Average rate

The average rate is calculated as the pulse average between the first and last pulse in the time window.

The 95% confidence intervals assume a constant level of radiation over the averaging period.

Averaging can be indefinite, or limited to a time period set by the average timer.

### Dose

The dose is calculated from the number of pulses in the time window.

### History

The history is calculated from the instantaneous rate, sampled once per second.

## Random number generator

The random number generator generates 16 symbols per run. If additional symbols are needed, simply return to the menu and initiate a new run.

The generator works by comparing the time interval between two successive pulses. To prevent bias, every second bit is flipped. The generator stores random number data in a 256-bit buffer.

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Letters & numbers" consumes approximately 6 bits per symbol, "Full ASCII" consumes 7 bits, "20-sided dice" consumes 5 bits, "Hexadecimal", "Decimal" and "12-sided dice" consumes 4 bits, "8-sided dice" and "6-sided dice" consumes 3 bits, "4-sided dice" consumes 2 bits, and "Coin flip" consumes 1 bit.

To generate bits more quickly, use a radioactive source.

## Data logging

To log data using Rad Pro, simply select a data logging interval in the settings. The data will be automatically stored.

* On the Bosean FS-600 and Bosean FS-1000, Rad Pro can store up to 95880 data points. At normal radiation levels, this allows for 66 days of data at 1-minute intervals, 166 days at 5-minute intervals, 332 days at 10-minute intervals, 998 days at 30-minute intervals, and 1997 days at 60-minute intervals.
* On the FS2011, Rad Pro can store up to 31496 data points. At normal radiation levels, this allows for 21 days of data at 1-minute intervals, 54 days at 5-minute intervals, 109 days at 10-minute intervals, 328 days at 30-minute intervals, and 656 days at 60-minute intervals.

## radpro-tool

`radpro-tool` allows you to download data logs, log live data, submit data to radiation monitoring websites and sync the real-time clock.

To use `radpro-tool` you need to install [Python](https://www.python.org) on your computer and install the necessary requirements by running the following command in a terminal:

    pip install -r tools/requirements.txt

If you plan to submit data to radiation monitoring websites, configure the website's settings by editing the `tools/radpro-tool.py` script.

To get help about `radpro-tool`, run the following command in a terminal:

    python radpro-tool.py --help
