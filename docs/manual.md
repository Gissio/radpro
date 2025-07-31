# User's manual

## Features

* **Measurement display:** Large-digit view optimized for field use.
* **Pulse indication:** Configurable with sound, voice, vibration, pulse LED, and display flash, with optional threshold.
* **Alert indication:** Configurable with sound, voice, vibration, pulse LED, and display flash, with dismissible alerts.
* **Tube fault alarm:** Detects tube malfunction or saturation.
* **Measurement units:** Primary and secondary units, including Sievert, rem, cpm (counts per minute), and cps (counts per second).
* **Averaging options:** Instantaneous rate averaging (adaptive fast, adaptive precision, 1 minute, 30 seconds, 10 seconds) and configurable survey timer.
* **Geiger-Müller tube settings:** Adjustable sensitivity, dead-time compensation, and high-voltage settings.
* **Data Logging:** Offline and live logging with data compression, compatible with GeigerLog.
* **Additional Features:**
  * Keyboard lock mode.
  * Usage and performance statistics.
  * Geiger tube dead-time measurement.
  * Power-on self-test and safety watchdog.
  * `radpro-tool` for low-level computer access.
  * Game.
* **Language support:** 27 languages, including English, Spanish, Chinese, and more.
* **User interface:** Based on the [OpenBridge 5.0](https://www.openbridge.no/) design standard with anti-aliased text.

## Measurements

**Important:** Always power off the device using the keyboard to ensure accurate dose tracking. Avoid removing batteries to turn off the device.

Rad Pro supports the following measurement modes:

### Instantaneous rate

Calculates the instantaneous radiation rate by dividing the number of pulses, minus one, by the time between the first and last pulse in the averaging period.

The secondary view can be switched between:

* **Bar view:** Shows radiation levels on a [logarithmic](https://en.wikipedia.org/wiki/Logarithmic_scale) scale, with each tick representing a tenfold increase in radiation, highlighting warning and alarm zones.
* **Time view:** Displays the averaging period duration.
* **Max view:** Shows the highest recorded rate.
* **Rate view:** Displays the instantaneous rate in secondary units.

The [confidence interval](https://en.wikipedia.org/wiki/Confidence_interval) indicates the range where the true rate lies with 95% probability, assuming stable radiation levels. For example, a rate of 1.000 µSv/h with a ±40% interval means the true rate is likely between 0.600 µSv/h and 1.400 µSv/h. A narrower interval reflects higher precision.

To reset the instantaneous rate or dismiss alerts, refer to the device's installation instructions.

### Average rate

Calculates the average radiation rate over a set period, using the same formula as the instantaneous rate.

The secondary view can be switched between:

* **Time view:** Shows the averaging period duration.
* **Rate view:** Displays the average rate in secondary units.
* **Dose view:** Shows the dose accumulated during the averaging period.

The confidence interval reflects the range where the true average rate lies with 95% probability, assuming stable radiation. Averaging can be indefinite or limited by time or confidence level. An alert signals when averaging completes.

For example, a 1-minute background radiation measurement might show 0.210 µSv/h with a ±36% interval (0.134–0.286 µSv/h). A 30-minute measurement could yield 0.154 µSv/h with a ±7.7% interval with greater precision.

To reset the average rate, refer to the device’s installation instructions.

### Cumulative dose

The cumulative dose tracks the total number of pulses recorded durent the measurement period.

The secondary view can be switched between:

* **Time view:** Shows the measurement period duration.
* **Dose view:** Displays the cumulative dose in secondary units.

To reset the cumulative dose or dismiss alerts, refer to the device’s installation instructions.

### History

At startup, the history is loaded from the data log and updates every second using the instantaneous rate. The plot uses a [logarithmic scale](https://en.wikipedia.org/wiki/Logarithmic_scale), with each division representing a tenfold increase in radiation, and includes warning and alarm zones.

The view can be switched between 10-minute, 1-hour, 1-day, 1-month, or 1-year views.

To clear the history, reset the data log.

## Sensitivity

**Sensitivity** measures how effectively a Geiger-Müller tube detects radiation, expressed as counts per minute (cpm) per microsievert per hour (µSv/h). Rad Pro provides default sensitivity settings for common tubes:

* J305, J321, M4011: 153.8 cpm/µSv/h
* J613, J614: 60.0 cpm/µSv/h
* SBM-20: 150.5 cpm/µSv/h

To set a custom sensitivity, go to **Settings** > **Geiger tube** > **Sensitivity**, and select a value. Accurate measurements require the correct sensitivity for the radiation type; otherwise, results may be inaccurate.

**Note:** For tubes with sensitivity ≥600 cpm/µSv/h, pulse indication is capped at 600 cpm/µSv/h to avoid excessive audio feedback, without affecting measurement accuracy.

## Data logging

Rad Pro logs cumulative dose count, from which rate and dose can be derived.

To start logging:

1. Ensure the device's time zone, date and time are set correctly.
2. Enable data logging in **Settings**. Data logs automatically in the background.
3. When memory is full, older data is overwritten.

**USB connection:** Some devices require enabling **Data mode** in settings. Disable it after use to save power.

**Downloading/Logging:** Use [GeigerLog](https://sourceforge.net/projects/geigerlog/) to download logs or log live data. The CPS (counts per second) data is raw; use the **MvAvg** function to average it.

**Note:** Data logging pauses during downloads.

## Instantaneous rate averaging

Choose from these averaging options to balance response speed and precision:

* **Adaptive fast:** Averages the last 20 pulses, targeting a ±50% confidence interval. Ideal for general use. See response curves: [instantaneousaveraging-adaptivefast.ipynb](../tests/instantaneousaveraging-adaptivefast.ipynb)
* **Adaptive precision:** Extends **Adaptive fast** with a minimum 5-second averaging time for greater precision at high radiation levels, but slower response. See response curves: [instantaneousaveraging-adaptiveprecision.ipynb](../tests/instantaneousaveraging-adaptiveprecision.ipynb)
* **Fixed periods (1 minute, 30 seconds and 10 seconds):** Uses a fixed averaging window. If zero or one pulse is detected during the averaging period, the rate registers as zero.

## Instantaneous rate alerts

Alerts trigger only when the confidence interval falls below 75% to reduce false alarms.

## Dead-time compensation

[Dead time](https://en.wikipedia.org/wiki/Geiger%E2%80%93M%C3%BCller_tube#Quenching_and_dead_time) is the brief period (50–200 µs) after a radiation event when the Geiger-Müller tube cannot detect another event, leading to undercounting at high radiation levels. Rad Pro compensates for these missed counts.

To measure dead time, go to **Settings** > **Statistics** and monitor the **Dead time** value. Measure several hours at normal radiation levels for accuracy, or use a radioactive source to speed up the process.

Rad Pro compensates dead time using the non-paralyzable model:

$$n = \frac{m}{1 - m \tau}$$

Where:

* $m$: Uncompensated rate (counts per seconds).
* $\tau$: Dead time (seconds).
* $n$: Compensated rate (counts per second).
  
The compensation factor is capped at 10 to prevent overflow. Compensation applies to instantaneous rate, average rate, cumulative dose, and history but not to tube life pulse count or data logs.

## HV profiles (supported devices)

High-voltage (HV) profiles control the voltage supplied to the Geiger-Müller tube, balancing power consumption and accuracy. Preconfigured profiles are included, or you can create a custom profile.

**Warning:** Incorrect HV settings may damage the tube or switching transistor due to overvoltage or overcurrent.

Tu measure high voltage:

1. Use a high-quality multimeter (10 MΩ input impedance) set to the 20 V range.
2. Connect a clean 1 GΩ resistor in series to the multimeter’s positive terminal.
3. Connect the negative terminal to ground and the resistor’s free end to the tube’s anode.
4. Calculate voltage: Multimeter reading × 101 (approximate).
5. **Warning:** High voltage can be lethal.

**Custom HV Profile:** Set the [PWM](https://en.wikipedia.org/wiki/Pulse-width_modulation) frequency and duty cycle. Higher frequencies reduce voltage ripple but increase power use; lower frequencies save power but may reduce accuracy.

## Tube fault alarm

The device triggers an alarm if:

* No pulses are detected within a safety interval, indicating a faulty high-voltage generator or tube. The safety interval $t_s$ (in seconds) is
$t_s = \frac{12000}{S}$, where $S$ is the tube’s sensitivity (cpm/µSv/h).
* The tube is saturated (due to high radiation) or shorted (due to malfunction).

## Random generator

The [random generator](https://en.wikipedia.org/wiki/Hardware_random_number_generator) uses pulse timing to produce random outcomes in multiple formats:

* **ASCII**, **alphanumeric**, **hexadecimal**, **decimal** and **binary**: up to 16 outcomes per run.
* **Die rolls**, **coin flips**: one outcome per run.

**Generation process:**

1. Measures time intervals between pulses, inverting every second bit to eliminate bias.
2. Stores bits in a 128-bit buffer.
3. Processes bits using the Fast Dice Roller algorithm.
4. Bit usage varies: ~7 bits (ASCII, 100-sided die), ~6 bits (alphanumeric), ~4 bits (hexadecimal, decimal, 10/12-sided die), ~3 bits (6/8-sided die), ~2 bits (4-sided die), or 1 bit (binary, coin flip).

**Tip:** Use a radioactive source for faster bit generation.

## radpro-tool

`radpro-tool` enables low-level device access from a computer for tasks like downloading logs, live logging, submitting data to radiation monitoring websites, retrieving device info, setting HV profiles (on supported devices), and syncing the clock.

**Setup:**

1. Install [Python](https://www.python.org) and [PIP](https://pip.pypa.io/en/stable/).
2. Install requirements:

   ```bash
   pip install -r tools/requirements.txt
   ```

**Commands**:

- Get help:

  ```bash
  python tools/radpro-tool.py --help
  ```
- Sync clock (e.g., COM13):

  ```bash
  python tools/radpro-tool.py --port COM13
  ```
- Download data log to `datalog.csv`:

  ```bash
  python tools/radpro-tool.py --port COM13 --download-datalog datalog.csv
  ```
- Log pulse data live to `live.csv` every minute:

  ```bash
  python tools/radpro-tool.py --port COM13 --log-pulsedata live.csv --period 60
  ```
- Submit data to [gmcmap.com](https://gmcmap.com):

  ```bash
  python tools/radpro-tool.py --port COM13 --submit-gmcmap [USER_ACCOUNT_ID] [GEIGER_COUNTER_ID]
  ```

## Data communications

Refer to the [communications protocol description](comm.md) for USB serial port communication details.

## FAQ

**Q: How do I reset average rate or cumulative dose?**

**A:** Check the keyboard mapping in your device’s installation instructions.

**Q: Why does my device’s date and time reset on power-on?**

**A:** The real-time clock (RTC) may lose power. For example, on the FNIRSI GC-01, replace the CR1220 backup battery.

**Q: Why does the splash screen last up to 60 seconds?**

**A:** The microprocessor may be initializing the RTC. If persistent, check the RTC battery (see above).

**Q: Why are instantaneous rate measurements noisy?**

**A:** Noise is inherent in short averaging periods. Select a longer averaging option (e.g., 1 minute) for smoother results, at the cost of slower response.

**Q: Why don’t I hear clicks?**

**A:** If a pulse threshold is set, clicks only occur above it. Disable the threshold in Settings to hear all counts.

**Q: Why aren’t rate alerts triggering?**

**A:** Alerts trigger only when the confidence interval is below 75% to avoid false alarms.

**Q: Why doesn’t the device click for every count?**

**A:** For tubes with sensitivity ≥600 cpm/µSv/h, pulse indication is capped at 600 cpm/µSv/h to prevent excessive audio feedback, without affecting measurements.
