# User's manual

## Features

* Measurement view with large digits, ideal for field application.
* Configurable pulse indication with optional threshold: sound (off, clicks, chirps, beeps), vibration (on supported devices), pulse LED (on supported devices), display flashes (on display sleep).
* Configurable alarm signaling: sound, vibration, pulse LED and display flashes.
* Ability to dismiss alarms.
* Tube fault alarm.
* Multiple measurement units: Sievert, rem, cpm (counts per minute), cps (counts per second).
* Configurable instantaneous rate averaging (adaptive fast, adaptive precision, 1 minute, 30 seconds and 10 seconds).
* Configurable average timer for performing surveys.
* Customizable Geiger-Müller tube settings: sensitivity, dead-time compensation, high voltage generator PWM frequency and duty cycle (on supported devices).
* Optional overrange alarm when dead-time compensation exceeds 10%.
* Preconfigured high voltage profiles (on supported devices).
* Offline and live data logging with data compression.
* Compatibility with the [GeigerLog](https://github.com/Gissio/geigerlog-radpro) data logging software.
* Keyboard lock mode.
* Statistics for tracking device usage and state.
* Geiger tube dead-time measurement.
* Game: nuclear chess (on supported devices).
* User interface based on the industry-standard [OpenBridge 5.0](https://www.openbridge.no/) design system.
* Anti-aliased text rendering on color screens.
* `radpro-tool` for low-level access to the device from a computer.
* Power-on self-test and safety watchdog.

## Measurements

For proper tracking of dose, always power off the device using the keyboard. Avoid removing the batteries to power off the device.

Rad Pro supports the following measurement modes:

### Instantaneous rate

The instantaneous rate is estimated by dividing the number of pulses within the instantaneous rate averaging period, minus one, by the time between the first and last pulse within that period.

The secondary view can be switched between an instantaneous rate bar view ([logarithmic](https://en.wikipedia.org/wiki/Logarithmic_scale), with each tick representing a tenfold increase in radiation level, and 1 µSv/h and 10 µSv/h alert zones), a time view that displays the length of the averaging period, an instantaneous rate max view, and an instantaneous rate cpm (counts per minute) view.

The [confidence interval](https://en.wikipedia.org/wiki/Confidence_interval) estimates the range of values that contain the true, actual instantaneous rate with a 95% probability, assuming a constant level of radiation.

An example: Imagine measuring an instantaneous radiation rate of 1.000 µSv/h with a confidence interval of ±40%. This means there's a 95% probability that the true radiation level lies between 0.600 µSv/h and 1.400 µSv/h. In other words, there's a 5% chance the true level falls outside this range. As the confidence interval narrows (becomes smaller), the measurement becomes more precise and certain.

To reset the instantaneous rate or dismiss a rate alarm, read the device's installation instructions.

### Average rate

The average rate is estimated by dividing the number of pulses within the averaging period, minus one, by the time between the first and last pulse within that period.

The secondary view can be switched between a time view that displays the length of the averaging period, an average rate cpm (counts per minute) view, and a counts view.

The confidence interval assumes a constant level of radiation over the averaging period.

Averaging can be indefinite, or limited by a configurable time or confidence level. When averaging finishes, the device will flash and emit a beep, signaling the completion of the measurement.

To reset the average, read the device's installation instructions.

An example: Imagine measuring background radiation. A one-minute average yields 0.210 µSv/h with a ±36% confidence interval. This means there's a 95% chance the true radiation level lies between 0.134 µSv/h and 0.286 µSv/h. Finding this interval too wide, you repeat the measurement with a 30-minute average. The new measurement is 0.154 µSv/h, now with a much narrower ±7.7% confidence interval.

### Cumulative dose

The cumulative dose is calculated from the number of pulses in the time period.

The secondary view can be toggled between a time view, showing the duration of the measurement period, and a counts view, showing the pulse count.

To reset the cumulative dose or dismiss a dose alarm, refer to the device's installation instructions.

### History

The history is calculated from the instantaneous rate, sampled once per second. The plot uses a [logarithmic scale](https://en.wikipedia.org/wiki/Logarithmic_scale), where each division represents a tenfold increase in radiation level. Alert zones at 1 µSv/h and 10 µSv/h are visually indicated.

The view can be toggled between a 10-minute view, 60-minute view or 24-hour view.

To reset the history, consult the device's installation instructions.

## Sensitivity

A Geiger tube's **sensitivity** measures how effectively it detects and responds to ionizing radiation. It is typically expressed as counts per minute (cpm) per unit of radiation rate (for instance, microsieverts per hour).

Rad Pro includes default sensitivity settings for various Geiger-Müller tubes, suitable for most general applications:

* J305: 153.8 cpm/µSv/h
* J321: 153.8 cpm/µSv/h
* J613: 60.0 cpm/µSv/h
* J614: 60.0 cpm/µSv/h
* M4011: 153.8 cpm/µSv/h
* SBM-20: 150.5 cpm/µSv/h

You can also set a custom sensitivity value by going to the settings, selecting "Geiger tube", "Sensitivity", and choosing an appropriate value from the list.

It is important to note that for common Geiger tubes, radiation rate and dose estimates are accurate only if the radiation source is known and the correct sensitivity value is used. Without this, measurements can be significantly inaccurate, as sensitivity varies with radiation type.

When using tubes with a sensitivity of 600 cpm/µSv/h or higher, Rad Pro caps the sensitivity for pulse indication at 600 cpm/µSv/h to provide approximately one pulse per second at background levels of radiation and prevent excessive audio feedback. This does not impact the actual measurement.

## Data logging

Rad Pro lets you log cumulative dose count, from which both rate and dose can be derived.

To start logging, ensure that the time zone, date and time of your device are correctly set, and enable data logging in the settings. Data is automatically logged in the background. When the memory becomes full, older data is overwritten.

To establish a USB data connection, some devices require enabling "Data mode" in the settings. To save power, disable "Data mode" when you are finished.

To download the data logs or log data live to a computer, use the [GeigerLog](https://github.com/Gissio/geigerlog-radpro) data logging software. "CPS" data is the low-level counts per second value. To average the data, use the "MvAvg" function.

Please note that data is not logged during the download process.

## Instantaneous rate averaging

Rad Pro offers several options for adjusting the instantaneous rate averaging period:

* **Adaptive fast:** Averages the last 20 pulses, aiming for a confidence interval of ±50%. This option is well-suited for general users. You can view the "Adaptive fast" response curves (radiation level vs. response time) here: [instantaneousaveraging-adaptivefast.ipynb](../tests/instantaneousaveraging-adaptivefast.ipynb)
* **Adaptive precision:** Builds on "Adaptive fast", adding a minimum averaging time of 5 seconds. With high levels of radiation, it delivers greater precision than "Adaptive fast" at the expense of slower response. You can view the "Adaptive precision" response curves here: [instantaneousaveraging-adaptiveprecision.ipynb](../tests/instantaneousaveraging-adaptiveprecision.ipynb)
* **Fixed periods (1 minute, 30 seconds and 10 seconds):** These options use a fixed averaging period and are only useful for specific use cases. Note that if zero or one pulse is detected during the averaging period, the instantaneous rate will register as zero.

## Instantaneous rate alarm

To minimize false alarms, the instantaneous rate alarm triggers only when the confidence interval falls below 75%.

## Dead time and dead-time compensation

[Dead time](https://en.wikipedia.org/wiki/Geiger%E2%80%93M%C3%BCller_tube#Quenching_and_dead_time) is the period of time during which the Geiger-Müller tube is unable to detect another radiation event immediately after detecting one. This occurs because the tube becomes saturated after each radiation event, typically for 50-200 µs. Consequently, measurements of high levels of radiation will be inaccurate as the tube fails to register the counts during the dead-time period.

Rad Pro's dead-time compensation lets you add these missed counts.

To apply dead-time compensation, you must first measure the tube's dead time. In Rad Pro, navigate to "Settings" > "Statistics", and monitor the "Dead time" value. Dead time is estimated by measuring the shortest time interval between pulses. For reliable results, measure over several hours under normal radiation levels. Using a radioactive source can speed up this process.

Rad Pro uses the non-paralyzable model for dead-time compensation:

$$n = \frac{m}{1 - m \tau}$$

Here, $m$ is the uncompensated rate in counts per seconds, $\tau$ is the tube's dead time in seconds and $n$ is the compensated rate in counts per second. To prevent overflow, the compensation factor $1/(1 - m\tau)$ is limited to a maximum value of 10.

Dead-time compensation is applied to instantaneous rate, average rate, cumulative dose and history. It is not applied to tube life pulse count nor data logs.

An optional "Overrange" alarm can be set to trigger when dead-time correction exceeds 10%.

## HV profiles

(on supported devices)

HV profiles let you control the high voltage supplied to the Geiger-Müller tube. Rad Pro includes several pre-configured profiles that let you balance power consumption and accuracy.

You can also define your own HV profile. **WARNING:** Wrong profile settings may damage the tube from overvoltage, as well as the switching transistor from overcurrent.

Setting up a custom HV profile requires measuring the high voltage at the tube. To accomplish this, connect a 1 GΩ resistor in series to the positive terminal of a high-quality multimeter (with a 10 MΩ input impedance). Ensure the resistor is clean to prevent spurious currents. Set the multimeter to the 20 V range. Connect the negative terminal of the multimeter to ground, and the free end of the resistor to the tube's anode. The high voltage corresponds approximately to the multimeter reading multiplied by a factor of (1,000 MΩ + 10 MΩ) / 10 MΩ = 101. **WARNING:** High voltage can be lethal.

An HV profile consists of a [PWM](https://en.wikipedia.org/wiki/Pulse-width_modulation) frequency and duty cycle. Typically, higher frequency values produce lower voltage ripple (voltage variations in time) but consume more power. Conversely, lower frequency values require less power, but may sacrifice measurement accuracy.

## Tube fault alarm

Rad Pro triggers a fault alarm if no pulses are detected within a certain safety interval. This may occur due to a malfunctioning high-voltage generator, or a defective Geiger-Müller tube. The safety interval $t_s$ (in seconds) is calculated based on the Geiger tube's sensitivity $S$ (in cpm/µSv/h) using the formula:

$$t_s = \frac{12000}{S}$$

Rad Pro also triggers a fault alarm if the Geiger-Müller tube becomes saturated due to high radiation levels, or shorted due to a malfunction.

## Random generator

The [random generator](https://en.wikipedia.org/wiki/Hardware_random_number_generator) offers multiple output formats. "Full ASCII", "Alphanumeric", "Hexadecimal", "Decimal" and "Binary" produce up to 16 outcomes per run, while die rolls and coin flips yield a single outcome per run. To generate additional outcomes, simply return to the menu and start a new run.

The generator produces random bits by measuring the time interval between consecutive pulses and comparing the results. To eliminate bias, every second bit is inverted. Random data is stored in a 128-bit buffer.

These bits are then processed with the [Fast Dice Roller](https://arxiv.org/abs/1304.1916) algorithm to produce random outcomes. Bit consumption varies by format: "Full ASCII" uses approximately 7 bits per outcome, "Alphanumeric" requires about 6 bits, "Hexadecimal" and "Decimal" each take around 4 bits, and "Binary" uses just 1 bit. For die rolls, "100-sided die" consumes about 7 bits, "20-sided die" uses roughly 5 bits, "12-sided die" and "10-sided die" each need around 4 bits, "8-sided die" and "6-sided die" require about 3 bits and "4-sided die" uses 2 bits. A "Coin flip" takes only 1 bit.

For faster bit generation, consider using a radioactive source.

## radpro-tool

`radpro-tool` gives you low-level access to your device from a computer, allowing you to download data logs, log data live, submit data live to radiation monitoring websites, get device information, set custom HV profiles (on supported devices) and sync the device's clock.

To use `radpro-tool`, install [Python](https://www.python.org) and [PIP](https://pip.pypa.io/en/stable/). You must also install the necessary requirements by running the following command in a terminal:

    pip install -r tools/requirements.txt

To get help about `radpro-tool`, run the following command in a terminal:

    python tools/radpro-tool.py --help

To sync the clock of a Rad Pro device connected to COM13:

    python tools/radpro-tool.py --port COM13

To download the data log to the file `datalog.csv`:

    python tools/radpro-tool.py --port COM13 --download-datalog datalog.csv

To log pulse data live to the file `live.csv` every minute:

    python tools/radpro-tool.py --port COM13 --log-pulsedata live.csv --period 60

To submit data live to the https://gmcmap.com website:

    python tools/radpro-tool.py --port COM13 --submit-gmcmap [USER_ACCOUNT_ID] [GEIGER_COUNTER_ID]

## Data communications

To communicate with Rad Pro through a USB serial port, read the [communications protocol description](comm.md).

## FAQ

**Q: How can I reset the average rate and cumulative dose measurements?**

**A:** Refer to the keyboard mapping section of your device's installation instructions for guidance on resetting measurements.

**Q: Why does my device's date and time reset every time I turn it on?**

**A:** Most likely your device's date and time reset because its real-time clock (RTC) loses power when turned off. On the FNIRSI GC-01, this might be due to a failing backup battery (a CR1220). Replacing this battery should fix the issue.

**Q: When I power on my device, why does Rad Pro stay on the splash screen for so long (up to 60 seconds)?**

**A:** The extended splash screen time could be due to the microprocessor initializing the real-time clock. If this delay persists, check the previous answer for troubleshooting steps.

**Q: Why are my instantaneous rate measurements so noisy?**

**A:** There's a balance between noise and response speed in instantaneous rate measurements. To reduce noise, choose an averaging option that extends the measurement period, though this will slow the response time.

**Q: I enabled clicks but I'm not hearing any.**

**A:** If a pulse threshold is set, you'll only hear clicks when radiation levels exceed this threshold. Make sure this setting isn't active if you expect to hear clicks for all counts.

**Q: Why isn’t the rate alarm triggering?**

**A:** The rate alarm is designed to avoid false alarms by only triggering when the confidence interval drops below 75%.

**Q: My device is not clicking for every count.**

**A:** If you select a Geiger tube sensitivity exceeding 600 cpm/µSv/h, Rad Pro caps the sensitivity for pulse indication at 600 cpm/µSv/h to prevent excessive audio feedback.
