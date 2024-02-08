# Rad Pro User's manual

## Features

* Multiple measurement units: Sievert, rem, counts per minute (cpm) and counts per second (cps).
* Configurable average timer.
* Overflow alarms.
* Configurable pulse click sounds: off, quiet, loud.
* Configurable display options: backlight (on monochrome displays) or theme, brightness level and sleep (on color displays).
* Configurable battery type selection (on devices with a removable battery).
* Real-time clock data logging.
* Customizable Geiger-Müller settings: conversion factor, dead-time compensation and HV duty cycle for tube high-voltage control.
* Dead-time measurement.
* Life statistics for tracking of device usage.
* Up to 80% less energy use (depending on the device).
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

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

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Alphanumeric" consumes approximately 6 bits per symbol, "Full ASCII" consumes 7 bits, "20-sided dice" consumes 5 bits, "Hexadecimal", "Decimal" and "12-sided dice" consumes 4 bits, "8-sided dice" and "6-sided dice" consumes 3 bits, "4-sided dice" consumes 2 bits, and "Coin flip" consumes 1 bit.

To generate bits more quickly, use a radioactive source.

## Data logging

To log data using Rad Pro, simply select a data logging interval in the settings. The data will be automatically stored.
