# User's manual

## Features

* Multiple measurement units: Sievert, rem, counts per minute (cpm) and counts per second (cps).
* Overflow and overload alerts.
* Configurable pulse click sounds: off, quiet, loud.
* Configurable backlight: off, on for 10 seconds, on for 60 seconds, pulse flashes or always on.
* Configurable battery type selection for accurate battery level monitoring.
* Support for multiple Geiger-Müller tubes: HH614 (48 mm long), M4011 (105 mm long), SBM-20 (108 mm long), SI-3BG (50 mm long).
* Life statistics to track usage and performance of the device.
* Up to 40% more battery life compared to the original firmware.
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Usage

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
