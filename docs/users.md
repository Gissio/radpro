# User's manual

## Features

* Multiple measurement units: Sievert, rem, cpm (counts per minute), cps (counts per second).
* Adaptive averaging window, aiming for a confidence interval below ±50 %.
* Configurable average timer for performing surveys.
* Data logging with data compression.
* Serial port/USB data communications.
* Compatibility with the [GeigerLog](https://github.com/Gissio/geigerlog-radpro) data logging software.
* Configurable pulse click sounds: off, quiet, loud.
* Dead time measurement.
* Customizable Geiger-Müller tube settings: conversion factor, dead-time compensation, high voltage generator PWM frequency and duty cycle (for tube voltage control).
* Configurable high voltage profiles.
* Overflow alarms.
* Statistics for tracking device usage and state.
* `radpro-tool` for low-level access to the device from a computer.
* User interface with the [OpenBridge 4.0](https://www.openbridge.no/) design system and anti-aliased text rendering on color screens.
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Measurements

For proper dose tracking, always power off the device using the keyboard. Avoid removing the batteries to power off the device.

Rad Pro supports the following measurement modes:

### Instantaneous rate

The instantaneous rate is estimated by dividing the number of pulses within a time window, minus one, by the time between the first and last pulse within that window.

The instantaneous time is the length of the time window used for calculating the instantaneous rate.

The [confidence interval](https://en.wikipedia.org/wiki/Confidence_interval) estimates the range of values that contain the true, actual instantaneous rate with a 95 % probability, assuming a constant level of radiation.

The time window is adaptively adjusted, aiming for a confidence interval below ±50 %. On radiation levels above 1 µSv/h, the time window is limited to 5 seconds to ensure a fast response.

An example: suppose you measure an instantaneous rate of 1.0 µSv/h with a confidence interval of ±40 %. This means that the true, actual level of radiation has a 95 % probability of falling within the interval [0.6 µSv/h, 1.4 µSv/h] (40 % below and above the measured value). This also means that there is a 5 % chance that the true level of radiation is outside this interval. As the confidence interval becomes smaller, the certainty of your measurement increases.

### Average rate

The average rate is estimated by dividing the number of pulses within a time window, minus one, by the time between the first and last pulse within that window.

The average time is the length of the time window used for calculating the instantaneous rate.

The confidence interval assumes a constant level of radiation over the averaging period.

Averaging can be continuous or limited by the average timer. Upon timer expiration, the device will flash and emit a beep, signaling the completion of the measurement.

An example: suppose you averaged background radiation for 1 minute, resulting in a measurement of 0.210 µSv/h with a confidence interval of ±36 %. This means that the actual level of radiation has a 95 % probability of falling within the interval [0.134 µSv/h, 0.286 µSv/h] (36 % below and above the measured value). Suppose you consider this confidence interval too high, so you repeat the measurement with a 30-minute time window. Your new measurement is 0.154 µSv/h with a confidence interval of ±7.7 %, which you now consider much more acceptable.

### Dose

The dose is calculated from the number of pulses in the time window.

### History

The history is calculated from the instantaneous rate, sampled once per second.

## Conversion factor

The conversion factor specifies the relationship between cpm (counts per minute) and the level of radiation in µSv/h. It is specific to the type of radiation being detected and the particular characteristics of the tube.

Rad Pro comes with default conversion factors for various Geiger-Müller tubes:

* J305: 175.0 cpm/µSv/h
* J321: 153.0 cpm/µSv/h
* J613: 68.4 cpm/µSv/h
* J614: 68.4 cpm/µSv/h
* M4011: 153.0 cpm/µSv/h
* SBM-20: 175.0 cpm/µSv/h

You can also set a custom conversion factor by going to the settings, selecting "Geiger tube", "Conversion Factor", and choosing an appropriate value from the list.

## Background level measurement

Geiger-Müller tubes, being composed of matter, inherently contain atoms prone to radioactive decay. Consequently, the tubes themselves emit radiation. This intrinsic radiation will depend on factors such as tube type, size, and production batch.

Hence, it's important to account for this intrinsic radiation when comparing the background level of radiation measured by different devices, as it will influence the measurement.

## Data logging

Rad Pro lets you log cumulative dose count, from which both rate and dose can be derived.

To log data, simply select a data logging interval in the settings. Data is automatically logged in the background.

To live log data on a computer or download the datalogs, use the [GeigerLog](https://github.com/Gissio/geigerlog-radpro) data logging software.

## Dead time and dead-time compensation

[Dead time](https://en.wikipedia.org/wiki/Geiger%E2%80%93M%C3%BCller_tube#Quenching_and_dead_time) is the period of time during which the Geiger-Müller tube is unable to detect another radiation event immediately after detecting one. This occurs because the tube becomes saturated after each radiation event, typically for a time of 50-200 µs. Consequently, measurements of high levels of radiation will be inaccurate as the tube fails to register the counts during this dead time period.

Rad Pro lets you compensate these missed counts by applying dead-time compensation.

To use dead-time compensation you need to first measure the dead time. To do so, go to the settings, select "Statistics" and monitor the "Dead time" value until it stabilizes. This process can take several hours under normal levels of radiation.

Dead-time compensation follows the non-paralyzable model:

$$n = \frac{m}{1 - m \tau}$$

where $m$ is the rate in counts per seconds, and $\tau$ is the tube's dead time in seconds. To prevent overflow, the compensation factor is limited to 10.

Dead-time compensation is applied at the beginning of the processing chain. Consequently the instantaneous rate, average rate, cumulative dose, tube pulse count and logged values all undergo dead-time compensation.

## HV profiles

HV profiles let you control the high voltage supplied to the Geiger-Müller tube. Rad Pro includes several pre-configured profiles that let you balance power consumption and accuracy.

You can also define your own HV profile. Be careful, as wrong profile settings MAY RESULT IN DAMAGE to both the tube from overvoltage and the switching transistor from overcurrent.

Setting up a custom HV profile requires measuring the high voltage at the tube. To accomplish this, connect a 1 GΩ resistor in series with a high-quality multimeter (with a 10 MΩ input impedance). Ensure the resistor is clean to prevent spurious currents. Set the multimeter to the 20 V range. The high voltage approximately corresponds to the multimeter reading multiplied by a factor of (1000 MΩ + 10 MΩ) / 10 MΩ = 101. Caution: high voltage CAN BE LETHAL.

An HV profile consists of a [PWM](https://en.wikipedia.org/wiki/Pulse-width_modulation) frequency and duty cycle. Typically, higher frequency values produce lower voltage ripple (voltage variations in time) but consume more power. Conversely, lower frequency values require less power, but may sacrifice measurement accuracy.

## Random generator

The [random generator](https://en.wikipedia.org/wiki/Hardware_random_number_generator) produces 16 symbols per run. If additional symbols are needed, simply return to the menu and initiate a new run.

The generator produces random bits by comparing the time interval between two successive pulses. To avoid bias, every second bit is flipped. The generator stores random data in a 128-bit buffer.

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Alphanumeric" uses approximately 6 bits per symbol, "Full ASCII" uses 7 bits, "20-sided dice" uses 5 bits, "Hexadecimal", "Decimal" and "12-sided dice" use 4 bits, "8-sided dice" and "6-sided dice" use 3 bits, "4-sided dice" uses 2 bits, and "Coin flip" uses 1 bit.

For faster bit generation, use a radioactive source.

## radpro-tool

`radpro-tool` gives you low-level access to your device, allowing you to download data logs, log live data on your computer, submit live data to radiation monitoring websites, get device information and sync the device's clock.

To use `radpro-tool`, install [Python](https://www.python.org) and the necessary requirements by running the following command in a terminal:

    pip install -r tools/requirements.txt

To get help about `radpro-tool`, run the following command in a terminal:

    python radpro-tool.py --help

To sync the clock of a Rad Pro device connected to COM13:

    python radpro-tool.py --port COM13

To download the datalog to the file `datalog.csv`:

    python radpro-tool.py --port COM13 --download-datalog datalog.csv

To live submit the level of radiation to the https://gmcmap.com website:

    python radpro-tool.py --port COM13 --submit-gmcmap [USER_ACCOUNT_ID] [GEIGER_COUNTER_ID]

## Data communications

To communicate with Rad Pro through a serial port or SWD (through an ST-LINK dongle), read the [communications protocol description](comm.md).
