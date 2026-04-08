# User's manual

## Features

* **Measurement display:** Large-digit view optimized for field use.
* **Pulse indication:** Configurable with sound, voice, vibration, pulse LED, and display flash, with optional threshold.
* **Alert indication:** Configurable with sound, voice, vibration, pulse LED, and display flash, with dismissible alerts.
* **Tube fault alarm:** Detects tube malfunction or saturation.
* **Measurement units:** Primary and secondary units, including Sievert, rem, cpm (counts per minute), and cps (counts per second).
* **Averaging options:** Instantaneous rate averaging (adaptive fast, adaptive precision, 1 minute, 30 seconds, 10 seconds) and configurable survey timer.
* **Geiger-Müller tube settings:** Adjustable sensitivity, dead-time compensation, and high-voltage settings.
* **Source compensation:** Pre-configured compensation for common gamma-emitting sources (Cs-137, Co-60, Am-241, etc.) to improve dose-rate accuracy when the source is known.
* **Data Logging:** Offline and live logging with data compression, compatible with GeigerLog.
* **Additional Features:**
  * Keyboard lock mode.
  * Usage and performance statistics.
  * Geiger tube dead-time measurement.
  * Power-on self-test and safety watchdog.
  * `radpro-tool` for low-level computer access.
  * Game.
* **Language support:** 30 languages, including English, Spanish, Chinese, and more.
* **User interface:** Based on the [OpenBridge 6.1](https://www.openbridge.no/) design standard, with anti-aliased text.

## Measurements

**Important:** Always power off the device using the keyboard to ensure accurate dose tracking. Avoid removing batteries to turn off the device.

Rad Pro supports the following measurement modes:

### Instantaneous rate

Calculates the instantaneous radiation rate by dividing the number of pulses, minus one, by the time interval between the first and last pulse within the averaging window.

The secondary view can be switched between:

* **Bar view:** Shows radiation levels on a [logarithmic](https://en.wikipedia.org/wiki/Logarithmic_scale) scale, with each tick representing a tenfold increase in radiation, highlighting warning and alarm zones.
* **Max view:** Shows the highest measured rate.
* **Rate view:** Displays the instantaneous rate in secondary units.
* **Cumulative view:** Displays the cumulative dose.

The [confidence interval](https://en.wikipedia.org/wiki/Confidence_interval) indicates the range where the true rate lies with 95% probability, assuming stable radiation levels. For example, a rate of 1.000 µSv/h with a ±40% interval means the true rate is likely between 0.600 µSv/h and 1.400 µSv/h. A narrower interval reflects higher precision.

To reset the instantaneous rate or dismiss alerts, refer to the device's installation instructions.

### Average rate

Calculates the average radiation rate over a time window, using the same formula as the instantaneous rate.

The secondary view can be switched between:

* **Time view:** Shows the averaging time.
* **Rate view:** Displays the average rate in secondary units.
* **Dose view:** Shows the dose accumulated during the averaging time.

The confidence interval reflects the range where the true average rate lies with 95% probability, assuming stable radiation. Averaging can be indefinite or limited by time or confidence level. An alert signals when averaging completes.

For example, a 1-minute background radiation measurement might show 0.210 µSv/h with a ±36% interval (0.134–0.286 µSv/h). A 30-minute measurement could yield 0.154 µSv/h with a ±7.7% interval with greater precision.

To reset the average rate, refer to the device’s installation instructions.

### Cumulative dose

The cumulative dose tracks the total number of pulses recorded during the measurement time.

The secondary view can be switched between:

* **Time view:** Shows the measurement time.
* **Dose view:** Displays the cumulative dose in secondary units.
* **Instantaneous view:** Displays the instantaneous rate.

To reset the cumulative dose or dismiss alerts, refer to the device’s installation instructions.

### History

At startup, the history is loaded from the data log and updates every second using the instantaneous rate. The plot uses a [logarithmic scale](https://en.wikipedia.org/wiki/Logarithmic_scale), with each division representing a tenfold increase in radiation, and includes warning and alarm zones.

The view can be switched between 10-minute, 1-hour, 1-day, 1-week, 1-month, or 1-year views.

To clear the history, reset the data log.

### Electric field (supported devices)

Calculates the electric field strength over a 400 ms window.

The secondary view can be switched between:

* **Max view:** Shows the highest measured value.
* **Magnetic field:** Shows the magnetic field strength.

To reset the electric field measurements or dismiss alerts, refer to the device’s installation instructions.

**Important:** The device's built-in **buzzer** and **USB connection** can interfere with electric field measurements. For highest accuracy, disable audio feedback and disconnect USB during measurements.

### Magnetic field (supported devices)

Calculates the magnetic field strength over a 400 ms window.

The secondary view can be switched between:

* **Max view:** Shows the highest measured value.
* **Electric field:** Shows the electric field strength.

To reset the magnetic field measurements or dismiss alerts, refer to the device’s installation instructions.

## Sensitivity

**Sensitivity** indicates how effectively a Geiger-Müller tube detects ionizing radiation. It is usually expressed in **counts per minute (cpm) per microsievert per hour (µSv/h)** — in other words, how many counts are registered for a given rate.

Rad Pro includes default sensitivity values derived from numerical simulations using [Rad Lab](https://github.com/Gissio/radlab). These values are calculated for a standard Cs-137 gamma source and apply to the most commonly used GM tubes:

* J305: 135.200 cpm/µSv/h
* M4011: 108.345 cpm/µSv/h
* J321: 108.345 cpm/µSv/h
* HH614: 30.157 cpm/µSv/h
* СБМ20 (SBM20): 106.105 cpm/µSv/h
* СИЗБГ (SI3BG): 3.267 cpm/µSv/h
* LND 7317: 252.567 cpm/µSv/h

To set a custom sensitivity value, go to **Settings > Geiger tube > Sensitivity** and select the appropriate value.

**Important:** Accurate dose-rate readings require the correct sensitivity for the radiation type and energy being measured. Using an incorrect value will lead to inaccurate results.

**Note:** For high sensitivity tubes (≥ 600 cpm/µSv/h), pulse indication is capped at the equivalent of 600 cpm/µSv/h to prevent uncomfortable clicking. This limit affects sound feedback only and does not reduce the accuracy of the measurements.

## Source compensation

When the source of ionizing radiation is known, **Rad Pro** can improve measurement accuracy by applying **source compensation**. This corrects for the energy-dependent response of Geiger–Müller tubes.

**This works only for gamma emitters** because beta radiation changes the spectrum with distance (beta particles are attenuated and their energy spectrum shifts as they travel through air, whereas gamma rays are far more penetrating and maintain a more consistent spectrum).

To use it, go to **Settings > Measurements >w Source compensation** and select the appropriate source from the list.

Notes:

* Compensation assumes **gamma-only** emissions (or situations where beta is negligible).
* To exclude beta radiation (especially important for mixed sources), place a metal shield between the source and the detector. For casual or home use, a simple and effective option is to place the detector inside a **stainless steel pan**: this typically blocks most beta particles while transmitting gamma rays with only minor attenuation.
* All compensation factors are relative to **Cs-137** (set as 1.0).
* Values are derived from numerical simulations performed with **[Rad Lab](https://github.com/Gissio/radlab)**.

### Compensation factors by tube and source

|Source|J305|M4011/J321|HH614|СБМ20 (SBM20)|СИЗБГ (SI3BG)|LND 7317|
|-|-|-|-|-|-|-|
|Cs-137|1|1|1|1|1|1|
|Co-60|1.114|1.119|1.371|0.791|1.397|0.998|
|Tc-99m|6.38|6.401|2.567|3.267|2.419|2.466|
|I-131|0.99|0.998|0.761|0.918|0.762|0.943|
|Lu-177|3.487|3.516|1.109|1.424|1.104|1.443|
|Am-241|11.925|11.88|4.468|6.774|0.554|3.123|
|Radium|1.062|1.066|0.979|0.869|0.991|0.99|
|Uranium ore|1.21|1.214|0.973|0.961|0.978|1.068|
|Uranium glass|1.367|1.37|0.973|1.054|1.015|1.123|
|Depleted uranium|1.353|1.356|1.089|1.05|1.1|1.118|
|Thorium ore|1.113|1.118|0.416|0.858|0.697|1.011|
|X-rays (60 kV)|5.486|5.465|2.424|5.686|0.125|2.806|
|K-40|1.052|1.058|1.398|0.735|1.478|0.987|
|Background radiation|1.338|1.345|1.099|1.012|1.028|1.15|

## Data logging

Rad Pro logs cumulative dose count, from which rate and dose can be derived.

To start logging:

1. Ensure the device's time zone, date and time are set correctly.
2. Enable data logging in **Settings**. Data logs automatically in the background.
3. When memory is full, older data is overwritten.

**USB connection:** Some devices require enabling **Data mode** in settings. Disable it after use to save power.

**Downloading/logging:** Use [GeigerLog](https://sourceforge.net/projects/geigerlog/) to download logs or log live data. The CPS (counts per second) data is raw; use the **MvAvg** function to average it.

**Note:** Data logging pauses during downloads.

## Instantaneous rate averaging

Choose from these averaging options to balance response speed and precision:

* **Adaptive fast:** Averages the last 20 pulses, targeting a ±50% confidence interval. Ideal for general use. See response curves: [instantaneousaveraging-adaptivefast.ipynb](../tests/instantaneousaveraging-adaptivefast.ipynb)
* **Adaptive precision:** Extends **Adaptive fast** with a minimum 5-second averaging time for greater precision at high radiation levels, but slower response. See response curves: [instantaneousaveraging-adaptiveprecision.ipynb](../tests/instantaneousaveraging-adaptiveprecision.ipynb)
* **Fixed intervals (1 minute, 30 seconds and 10 seconds):** Uses a fixed averaging window. If zero or one pulse is detected during the averaging interval, the rate registers as zero.

## Instantaneous rate alerts

Alerts trigger only when the confidence interval falls below 75% to reduce false alarms.

## Dead-time compensation

[Dead time](https://en.wikipedia.org/wiki/Geiger%E2%80%93M%C3%BCller_tube#Quenching_and_dead_time) is the brief interval (50–200 µs) after a radiation event during which a Geiger-Müller tube cannot detect another event, leading to undercounting at high radiation levels. Rad Pro can compensate for these missed counts.

To measure dead time, go to **Settings > Statistics** and monitor the **Dead time** value. Measure several hours at normal radiation levels for accuracy, or use a radioactive source to speed up the process.

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

To measure high voltage:

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

To restart the generator, refer to the device's installation instructions.

**Generation process:**

1. Measures time of pulses with microsecond precision.
2. Extracts randomness from the two least significant bits of each timing measurement.
3. Stores bits in a 128-bit buffer.
4. Processes bits using the Fast Dice Roller algorithm.
5. Bit consumption depends on output format:
    * ~7 bits: ASCII, 100-sided die
    * ~6 bits: alphanumeric
    * ~4 bits: hexadecimal, decimal, 10/12-sided die
    * ~3 bits: 6/8-sided die
    * ~2 bits: 4-sided die
    * 1 bit: binary, coin flip

**Tips:**

* Use a radioactive source for faster bit generation.
* Disable sound/vibration notifications in **Settings > Alerts** if you want silent operation when the generator runs to completion.

## radpro-tool

`radpro-tool` enables low-level device access from a computer for tasks like downloading logs, live logging, submitting data to radiation monitoring websites, retrieving device info, setting HV profiles (on supported devices), and syncing the clock.

**Setup:**

1. Install [Python](https://www.python.org) and [PIP](https://pip.pypa.io/en/stable/).
2. Install requirements:

   ```bash
   pip install -r tools/requirements.txt
   ```

**Commands**:

* Get help:

  ```bash
  python tools/radpro-tool.py --help
  ```

* Sync clock (e.g., COM13):

  ```bash
  python tools/radpro-tool.py --port COM13
  ```

* Download data log to `datalog.csv`:

  ```bash
  python tools/radpro-tool.py --port COM13 --download-datalog datalog.csv
  ```

* Log pulse data live to `live.csv` every minute:

  ```bash
  python tools/radpro-tool.py --port COM13 --log-pulsedata live.csv --period 60
  ```

* Submit data to [gmcmap.com](https://gmcmap.com):

  ```bash
  python tools/radpro-tool.py --port COM13 --submit-gmcmap [USER_ACCOUNT_ID] [GEIGER_COUNTER_ID]
  ```

* Submit data to [radmon.org](https://radmon.org):

  ```bash
  python tools/radpro-tool.py --port COM13 --submit-radmon [USERNAME] [DATA_SENDING_PASSWORD]
  ```

* Submit data to [safecast.org](https://safecast.org):

  ```bash
  python tools/radpro-tool.py --port COM13 --submit-safecast [API_KEY] [DEVICE_ID]
  ```

* Submit data to [safecast.org](https://safecast.org) with location data:

  ```bash
  python tools/radpro-tool.py --port COM13 --submit-safecast [API_KEY] [DEVICE_ID] --safecast-latitude 37.7749 --safecast-longitude -122.4194 --safecast-height 100.5
  ```

* Submit data to [opensensemap.org](https://opensensemap.org):

  ```bash
  python tools/radpro-tool.py --port COM13 --submit-opensensemap [SENSE_BOX_ID] [API_KEY] --opensensemap-sensor-id [SENSOR_ID]
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

**A:** Noise is inherent in short averaging intervals. Select a longer averaging window (e.g., 1 minute) for smoother results, at the cost of slower response.

**Q: Why don’t I hear clicks?**

**A:** If a pulse threshold is set, clicks only occur above it. Disable **Settings > Pulses > Threshold** to hear all counts.

**Q: Why aren’t rate alerts triggering?**

**A:** Alerts trigger only when the confidence interval is below 75% to avoid false alarms.

**Q: Why doesn’t the device click for every count?**

**A:** For tubes with sensitivity ≥600 cpm/µSv/h, pulse indication is capped at 600 cpm/µSv/h to prevent excessive audio feedback.
