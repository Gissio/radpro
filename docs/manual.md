# Rad Pro User's manual

## Features

* Multiple measurement units: Sievert, rem, counts per minute (cpm), counts per second (cps).
* Adaptive averaging window (aiming for a 50 % confidence interval).
* Fast response to abrupt rate changes.
* Configurable average timer for performing surveys.
* Overflow alarms.
* Data logging with data compression.
* Dead-time measurement.
* Customizable Geiger-Müller tube settings: conversion factor, dead-time compensation, high voltage generator frequency and duty cycle (for tube voltage control).
* Configurable pulse click sounds: off, quiet, loud.
* Statistics for tracking device usage and state.
* Configurable high voltage profiles, including an energy-saving profile that reduces power consumption by up to 80 %.
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Measurements

For proper dose tracking, always power off the device using the keyboard. Avoid removing the batteries to power off the device.

Rad Pro supports the following measurement modes:

### Instantaneous rate

The instantaneous rate is estimated by dividing the number of pulses within a time window, minus one, by the time between the first and last pulse within that window.

The confidence interval estimates the range of values that contain the true instantaneous rate with a 95 % probability, assuming a constant level of radiation.

The time window is adaptively adjusted to achieve a 50 % confidence interval.

For fast response to abrupt changes, Rad Pro also estimates the instantaneous rate with a 1-second window. If this fast rate falls beyond a four-sigma confidence interval (99.99 %) of the current instantaneous rate, the time window is reset.

### Average rate

The average rate is estimated by dividing the number of pulses within a time window, minus one, by the time between the first and last pulse within that window.

The 95 % confidence interval assumes a constant level of radiation over the averaging period.

Averaging can be indefinite, or limited by the average timer.

### Dose

The dose is calculated from the number of pulses in the time window.

### History

The history is calculated from the instantaneous rate, sampled once per second.

## Dead-time compensation

Dead-time compensation follows the non-paralyzable model:

$$n = \frac{m}{1 - m \tau}$$

where $m$ is the rate in counts per seconds, and $\tau$ is the tube's dead-time in seconds.

Dead-time compensation is applied at the beginning of the processing chain, thus instantaneous rate, average rate, cumulative dose, tube pulse count and logged values are all dead-time compensated.

## HV profiles

HV profiles let you control the high voltage applied to the Geiger-Müller tube. Rad Pro includes several pre-configured profiles, including a factory default profile with the original firmware's settings, an optimized profile that balances power consumption and accuracy, and an energy-saving profile that significantly reduces power consumption at the expense of lower accuracy at higher radiation levels.

You can also define your own HV profile. Be careful, as wrong profile values MAY DAMAGE both the tube due to overvoltage as well as the switching transistor due to overcurrent.

To set up a custom voltage profile you need to measure the high voltage at the tube. To do this, connect a 1 GΩ resistor in series to a multimeter with 10 MΩ input impedance. The resistor should be cleaned thorougly in order to avoid spurious currents. Set the multimeter to the 20 V range. The high voltage will approximately correspond to the voltmeter reading multiplied by a factor of 1000 MΩ / 10 MΩ = 100. Caution: high voltage CAN BE LETHAL.

An HV profile consists of a frequency and a duty cycle. Typically, higher frequency values produce lower voltage ripple (voltage variations in time) but consume more power. Conversely, lower frequency values require less power, but may sacrifice measurement accuracy.

## Random generator

The random generator produces 16 symbols per run. If additional symbols are needed, simply return to the menu and initiate a new run.

The generator produces random bits by comparing the time interval between two successive pulses. To avoid bias, every second bit is flipped. The generator stores random data in a 128-bit buffer.

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Alphanumeric" uses approximately 6 bits per symbol, "Full ASCII" uses 7 bits, "20-sided dice" uses 5 bits, "Hexadecimal", "Decimal" and "12-sided dice" uses 4 bits, "8-sided dice" and "6-sided dice" uses 3 bits, "4-sided dice" uses 2 bits, and "Coin flip" uses 1 bit.

For faster bit generation, use a radioactive source.

## Data logging

To log data using Rad Pro, select a data logging interval in the settings. Data is automatically stored.

## radpro-tool

`radpro-tool` lets you download data logs, log live data on your computer, submit live data to radiation monitoring websites, and sync the real-time clock.

To use `radpro-tool`, install [Python](https://www.python.org) on your computer and install the necessary requirements by running the following command in a terminal:

    pip install -r tools/requirements.txt

If you plan to submit data to radiation monitoring websites, configure the website's settings by editing the `tools/radpro-tool.py` script.

To get help about `radpro-tool`, run the following command in a terminal:

    python radpro-tool.py --help

## Communications

To communcate with Rad Pro through a serial port or ST-LINK dongle through SWD, check out the [description of the communications protocol](comm.md).
