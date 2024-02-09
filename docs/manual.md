# Rad Pro User's manual

## Features

* Multiple measurement units: Sievert, rem, counts per minute (cpm), counts per second (cps).
* Adaptive averaging window (aiming for 50 % confidence interval).
* Fast response (on abrupt instantaneous rate changes).
* Configurable average timer for performing surveys.
* Overflow alarms.
* Data logging with data compression.
* Dead-time measurement.
* Customizable Geiger-Müller tube settings: conversion factor, dead-time compensation (at cps level), high-voltage generator frequency and duty cycle (for tube voltage control).
* Configurable pulse click sounds: off, quiet, loud.
* Statistics for tracking device usage and state.
* Energy saving mode for reduced power consumption (up to 80 %).
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Measurements

For correct tracking of the dose, always power off the device using the keyboard. Avoid removing the batteries to power off the device.

Rad Pro supports the following measurement modes:

### Instantaneous rate

The instantaneous rate is estimated by dividing the number of pulses within an averaging window, minus one, by the time between the first and last pulse within that window.

The confidence interval estimates the range of values that contain the true instantaneous rate with a 95% probability, assuming a constant level of radiation.

The averaging window's length is calculated adaptively in order to aim the confidence interval at 50%.

For fast response, Rad Pro also estimates the instantaneous rate with a 1-second window. If this fast rate falls beyond a four-sigma confidence interval (99.99%) of the current instantaneous rate, the averaging window is reset.

### Average rate

The average rate is estimated by dividing the number of pulses within an averaging window, minus one, by the time between the first and last pulse within that window.

The 95% confidence intervals assume a constant level of radiation over the averaging period.

Averaging can be indefinite, or limited by the average timer.

### Dose

The dose is calculated from the number of pulses in the time window.

### History

The history is calculated from the instantaneous rate, sampled once per second.

## Dead-time compensation

Dead-time compensation is performed with the non-paralyzable model:

$$n = \frac{m}{1 - m \tau}$$

where $m$ is the rate in counts per seconds, and $\tau$ is the tube dead-time in seconds.

Dead-time compensation is applied at the cps level, at the beginning of the processing chain. Instantaneous rate, average rate, cumulative dose, tube pulse count and logged values are, thus, dead-time compensated.

## Random generator

The random generator generates 16 symbols per run. If additional symbols are needed, simply return to the menu and initiate a new run.

The generator works by comparing the time interval between two successive pulses. To avoid bias, every second bit is flipped. The generator stores random number data in a 128-bit buffer.

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Alphanumeric" uses approximately 6 bits per symbol, "Full ASCII" uses 7 bits, "20-sided dice" uses 5 bits, "Hexadecimal", "Decimal" and "12-sided dice" uses 4 bits, "8-sided dice" and "6-sided dice" uses 3 bits, "4-sided dice" uses 2 bits, and "Coin flip" uses 1 bit.

To generate bits more quickly, use a radioactive source.

## Data logging

To log data using Rad Pro, simply select a data logging interval in the settings. Data is automatically stored.

## radpro-tool

`radpro-tool` allows you to download data logs, log live data locally, submit live data to radiation monitoring websites and sync the real-time clock.

To use `radpro-tool` you need to install [Python](https://www.python.org) on your computer and install the necessary requirements by running the following command in a terminal:

    pip install -r tools/requirements.txt

If you plan to submit data to radiation monitoring websites, configure the website's settings by editing the `tools/radpro-tool.py` script.

To get help about `radpro-tool`, run the following command in a terminal:

    python radpro-tool.py --help
