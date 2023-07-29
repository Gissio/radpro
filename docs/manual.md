# Rad Pro User's manual

## Rad Pro features

* Multiple measurement units: Sievert, rem, counts per minute (cpm) and counts per second (cps).
* Overflow and overload alerts.
* Configurable pulse click sounds: off, quiet, loud.
* Configurable backlight: off, on for 10 seconds, on for 60 seconds, pulse flashes or always on.
* Real-time clock for data logging.
* Configurable battery type selection for accurate battery level monitoring.
* Support for multiple Geiger-Müller tubes with customizable conversion factor.
* Life statistics to track usage and performance of the device.
* Up to 40% more battery life compared to the original firmware.
* Power-on self-test and safety watchdog.
* Game: nuclear chess.

## Usage

* On the FS2011, the MENU/OK key functions as the ENTER key, and the PLAY/PAUSE key as the BACK key.

* To power the device on and off, long-press the POWER key.
* To activate the backlight (if enabled), press the POWER key.

* Use the UP and DOWN keys to change the mode or selection.
* In measurement modes, press the BACK key to toggle between holding and unholding the current measurement.
* To reset the measurements for the selected mode, long-press the BACK key.
* Press the ENTER key to access the menu.

* For accurate dose storage, always power off the device by long-pressing the POWER key. Avoid removing batteries to power off.

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

The random number generator produces one bit for every two new pulses by comparing the interval between them with a precision of 125 ns. The bit is discarded if the intervals are the same length. To prevent bias, every second bit is flipped. The generator stores entropy in a 128-bit buffer.

Random symbols are generated from the bits using the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm. "Letters & numbers" consumes approximately 6 bits per symbol, "Full ASCII" consumes 7 bits, "20-sided dice" consumes 5 bits, "Hexadecimal", "Decimal" and "12-sided dice" consumes 4 bits, "8-sided dice" and "6-sided dice" consumes 3 bits, "4-sided dice" consumes 2 bits, and "Coin flip" consumes 1 bit.

To generate bits more quickly, use a radioactive source.

## Data logging

To log data using Rad Pro, simply select a data logging interval in the settings. The data will be automatically stored.

On the FS2011, Rad Pro can store up to 3840 data points. This allows for 2.5 days of data at 1-minute intervals, 13 days at 5-minute intervals, 26 days at 10-minute intervals, 80 days at 30-minute intervals, and 160 days at 60-minute intervals.

To download the data from your device, follow these steps:

1. Install Python on your computer.
2. Install the necessary requirements by executing the following command in a terminal: `pip install -r tools/requirements.txt`.
3. Connect your device to the computer.
4. Download the data by running the `tools/datalog-download.py` script.

## Live data capture

To capture data live with Rad Pro, follow these steps:

1. Install Python on your computer.
2. Install the necessary requirements by executing the following command in a terminal: `pip install -r tools/requirements.txt`.
3. Connect your device to the computer.
4. Start capturing data by running the `tools/live-capture.py` script.

## Submission to radiation monitoring websites

To submit your data to a radiation monitoring website such as https://gmcmap.com or https://radmon.org, follow these steps:

1. Create an account on the chosen website.
2. Install Python on your computer.
3. Install the necessary requirements by executing the following command in the terminal: `pip install -r tools/requirements.txt`.
4. Configure your settings for https://gmcmap.com and/or https://radmon.org in the `tools/monitoring-website-client.py` file.
5. Connect your device to the computer.
6. Launch the radiation monitoring client by running the `tools/monitoring-website-client.py` script.
