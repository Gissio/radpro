# User's manual

## Features

* Multiple measurement units: Sievert, rem, cpm (counts per minute), cps (counts per second).
* Adaptive averaging window, aiming for a confidence interval below ±50 %.
* Configurable average timer for performing surveys.
* Data logging with data compression.
* Serial port/USB data communications.
* `radpro-tool` for downloading the data log, logging live data, submitting data to radiation monitoring websites and syncing the device clock.
* Configurable pulse click sounds: off, quiet, loud.
* Dead-time measurement.
* Customizable Geiger-Müller tube settings: conversion factor, dead-time compensation, high voltage generator PWM frequency and duty cycle (for tube voltage control).
* Configurable high voltage profiles.
* Overflow alarms.
* Statistics for tracking device usage and state.
* User interface with the [OpenBridge 4.0](https://www.openbridge.no/) design system and anti-aliased text rendering on color screens.
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Measurements

For proper dose tracking, always power off the device using the keyboard. Avoid removing the batteries to power off the device.

Rad Pro supports the following measurement modes:

### Instantaneous rate

The instantaneous rate is estimated by dividing the number of pulses within a time window, minus one, by the time between the first and last pulse within that window.

The confidence interval estimates the range of values that contain the true instantaneous rate with a 95 % probability, assuming a constant level of radiation.

The time window is adaptively adjusted, aiming for a confidence interval below ±50 %. On radiation levels above 1 µSv/h, the time window is limited to 5 seconds to ensure a fast response.

### Average rate

The average rate is estimated by dividing the number of pulses within a time window, minus one, by the time between the first and last pulse within that window.

The confidence interval assumes a constant level of radiation over the averaging period.

Averaging can be indefinite, or limited by the average timer.

### Dose

The dose is calculated from the number of pulses in the time window.

### History

The history is calculated from the instantaneous rate, sampled once per second.

## Conversion factor

The conversion factor specifies the relationship between cpm (pulse counts per minute) and µSv/h of a Geiger-Müller tube. 

Rad Pro's default conversion factors are:

* J305: 175.0 µSv/h/cpm
* J321: 153.0 µSv/h/cpm
* J613: 68.4 µSv/h/cpm
* J614: 68.4 µSv/h/cpm
* M4011: 153.0 µSv/h/cpm
* SBM-20: 175.0F µSv/h/cpm

## Dead-time compensation

Dead-time compensation follows the non-paralyzable model:

$$n = \frac{m}{1 - m \tau}$$

where $m$ is the rate in counts per seconds, and $\tau$ is the tube's dead-time in seconds.

Dead-time compensation is applied at the beginning of the processing chain, thus instantaneous rate, average rate, cumulative dose, tube pulse count and logged values are dead-time compensated.

## HV profiles

HV profiles let you control the high voltage applied to the Geiger-Müller tube. Rad Pro includes several pre-configured profiles that let you balance power consumption and accuracy.

You can also define your own HV profile. Be careful, as wrong profile settings MAY DAMAGE both the tube due to overvoltage as well as the switching transistor due to overcurrent.

To set up a custom HV profile you need to measure the high voltage at the tube. To do this, connect a 1 GΩ resistor in series to a high-quality multimeter (10 MΩ input impedance). The resistor should be cleaned thorougly in order to avoid spurious currents. Set the multimeter to the 20 V range. The high voltage will correspond approximately to the multimeter reading, multiplied by a factor of (1000 MΩ + 10 MΩ) / 10 MΩ = 101. Caution: high voltage CAN BE LETHAL.

An HV profile consists of a [PWM](https://en.wikipedia.org/wiki/Pulse-width_modulation) frequency and duty cycle. Typically, higher frequency values produce lower voltage ripple (voltage variations in time) but consume more power. Conversely, lower frequency values require less power, but may sacrifice measurement accuracy.

## Random generator

The random generator produces 16 symbols per run. If additional symbols are needed, simply return to the menu and initiate a new run.

The generator produces random bits by comparing the time interval between two successive pulses. To avoid bias, every second bit is flipped. The generator stores random data in a 128-bit buffer.

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Alphanumeric" uses approximately 6 bits per symbol, "Full ASCII" uses 7 bits, "20-sided dice" uses 5 bits, "Hexadecimal", "Decimal" and "12-sided dice" use 4 bits, "8-sided dice" and "6-sided dice" use 3 bits, "4-sided dice" uses 2 bits, and "Coin flip" uses 1 bit.

For faster bit generation, use a radioactive source.

## Data logging

To log data using Rad Pro, select a data logging interval in the settings. Data is automatically logged in the background.

## radpro-tool

`radpro-tool` lets you download data logs, log live data on your computer, submit live data to radiation monitoring websites, and sync the device's clock.

To use `radpro-tool`, install [Python](https://www.python.org) on your computer and install the necessary requirements by running the following command in a terminal:

    pip install -r tools/requirements.txt

If you plan to submit data to radiation monitoring websites, configure the website's settings by editing the `tools/radpro-tool.py` script.

To get help about `radpro-tool`, run the following command in a terminal:

    python radpro-tool.py --help

## Communications

To communicate with Rad Pro through a serial port or ST-LINK dongle through SWD, read the [communications protocol description](comm.md).
