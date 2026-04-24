# Rad Pro Reference Manual

This manual provides technical reference for the **Rad Pro** custom firmware.

## Measurements

### Instantaneous Rate

The **instantaneous radiation rate** is calculated by dividing the number of pulses, minus one, by the time interval between the first and last pulse within the averaging window.

### Average Rate

Calculates the **average radiation rate** over a time window, using the same formula as the instantaneous rate.

### Cumulative Dose

The **cumulative dose** tracks the total number of pulses recorded during the measurement time.

### History

At startup, the **history** is loaded from the data log and updates every second using the instantaneous rate.

To clear the history, go to **Settings > Data log > Reset**.

### Electric and Magnetic Field Strength (If Supported)

Calculates the **electric and magnetic field strength** over a 400 ms window.

**Important:** The device's built-in **buzzer** and **USB connection** can interfere with electric field measurements. For highest accuracy, disable audio feedback and disconnect USB during measurements.

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

**Note:** For high sensitivity tubes (≥ 600 cpm/µSv/h), pulse indication is capped at the equivalent of 600 cpm/µSv/h to prevent uncomfortable clicking. This limit affects pulse feedback only and does not reduce the accuracy of the measurements.

## Source Compensation

When the source of ionizing radiation is known, **Rad Pro** can improve measurement accuracy by applying **source compensation**. This corrects for the energy-dependent response of Geiger–Müller tubes.

**This works only for gamma emitters** because beta radiation changes the spectrum with distance (beta particles are attenuated and their energy spectrum shifts as they travel through air, whereas gamma rays are far more penetrating and maintain a more consistent spectrum).

To use it, go to **Settings > Measurements > Source compensation** and select the appropriate source from the list.

**Notes:**

* Compensation assumes **gamma-only** emissions (or situations where beta is negligible).
* To exclude beta radiation (especially important for mixed sources), place a metal shield between the source and the detector. For casual or home use, a simple and effective option is to place the detector inside a **stainless steel pan**: this typically blocks most beta particles while transmitting gamma rays with only minor attenuation.
* All compensation factors are relative to **Cs-137**.
* Values are derived from numerical simulations performed with **[Rad Lab](https://github.com/Gissio/radlab)**.

### Compensation Factors by Tube and Source

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

## Data Logging

To start logging:

1. Ensure the device's time zone, date and time are set correctly.
2. Enable data logging in **Settings**. Data logs automatically in the background.
3. When memory is full, older data is overwritten.

**USB connection:** Some devices require enabling **Data mode** in settings. Disable it after use to save power.

**Downloading/logging:** Use [GeigerLog](https://sourceforge.net/projects/geigerlog/) to download logs or log live data. The CPS (counts per second) data is raw; use the **MvAvg** function to average it.

**Note:** Data logging pauses during downloads.

## Instantaneous Rate Averaging

Choose from these averaging options to balance response speed and precision:

* **Adaptive fast:** Averages the last 20 pulses, targeting a ±50% confidence interval. Ideal for general use. See response curves: [instantaneousaveraging-adaptivefast.ipynb](../tests/instantaneousaveraging-adaptivefast.ipynb)
* **Adaptive precision:** Extends **Adaptive fast** with a minimum 5-second averaging time for greater precision at high radiation levels, but slower response. See response curves: [instantaneousaveraging-adaptiveprecision.ipynb](../tests/instantaneousaveraging-adaptiveprecision.ipynb)
* **Fixed intervals (1 minute, 30 seconds and 10 seconds):** Uses a fixed averaging window. If zero or one pulse is detected during the averaging interval, the rate registers as zero.

## Instantaneous Rate Alerts

Alerts trigger only when the confidence interval falls below 75% to reduce false alarms.

## Dead-Time Compensation

[Dead time](https://en.wikipedia.org/wiki/Geiger%E2%80%93M%C3%BCller_tube#Quenching_and_dead_time) is the brief interval (50–200 µs) after a radiation event during which a Geiger-Müller tube cannot detect another event, leading to undercounting at high radiation levels. Rad Pro can compensate for these missed counts.

To measure dead time, go to **Settings > Statistics** and monitor the **Dead time** value. Measure several hours at normal radiation levels for accuracy, or use a radioactive source to speed up the process.

Rad Pro compensates dead time using the non-paralyzable model:

$$n = \frac{m}{1 - m \tau}$$

Where:

* $m$: Uncompensated rate (counts per seconds).
* $\tau$: Dead time (seconds).
* $n$: Compensated rate (counts per second).
  
The compensation factor is capped at 10 to prevent overflow. Compensation applies to instantaneous rate, average rate, cumulative dose, and history but not to tube life pulse count or data logs.

## HV Profiles (If Supported)

High-voltage (HV) profiles control the voltage supplied to the Geiger-Müller tube, balancing power consumption and accuracy. Preconfigured profiles are included, or you can create a custom profile.

⚠️ Incorrect HV settings may damage the tube or switching transistor due to overvoltage or overcurrent.

Higher PWM frequency provides more stable voltage but increases power consumption. Lower PWM frequency reduces power consumption but may decrease measurement accuracy due to higher ripple.

The recommended HV for most supported tubes (M4011, J305, J321, HH614, SBM20) is **440 V**.

### Measuring HV with a Multimeter (Recommended)

⚠️ High voltage can be lethal.

1. Use a high-quality multimeter (10 MΩ input impedance) set to the 20 V range.
2. Connect a clean 1 GΩ resistor (or ten 10 MΩ resistors in series) in series with the multimeter's positive terminal.
3. Connect the negative terminal to ground and the free end of the resistor to the tube's anode.
4. Calculate voltage: Multimeter reading × 101 (or × 11 when using 10 MΩ resistors).

### Setting HV without a Multimeter (Lower Accuracy)

1. Set PWM frequency to 2.5 kHz.
2. Start with the lowest duty cycle and increase slowly until the tube begins producing counts (note this value).
3. Continue increasing slowly until the displayed rate exceeds background radiation (> 0.3 µSv/h).
4. Choose the average of the two duty-cycle values as the operating point.

⚠️ Leaving the counter running at the higher PWM duty cycle can damage the tube.

## Tube Fault Alarm

The device triggers an alarm if:

* No pulses are detected within a safety interval, indicating a faulty high-voltage generator or tube. The safety interval $t_s$ (in seconds) is
$t_s = \frac{12000}{S}$, where $S$ is the tube's sensitivity (cpm/µSv/h).
* The tube is saturated (due to high radiation) or shorted (due to malfunction).

## Random Generator

The **random number generator** uses pulse timing to produce random outcomes in multiple formats:

* **ASCII**, **alphanumeric**, **hexadecimal**, **decimal** and **binary**: up to 16 outcomes per run.
* **Die rolls**, **coin flips**: one outcome per run.

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

## Data Communications

Refer to the [communications protocol description](comm.md) for USB serial port communication details.

## Further Resources

* [Installation guides](../README.md#Installation) – For supported devices
* [Rad Pro user's manual](user-manual.md) – Easy guide to using Rad Pro
* [Ionizing radiation field guide](https://github.com/Gissio/ionizing-radiation-field-guide) – Learn about ionizing radiation
* [Rad Pro developers's manual](developers-manual.md) – Technical details for developers
