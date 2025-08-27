# Installing Rad Pro on GQ GMC-800 Geiger counters

To install the Rad Pro firmware on GQ GMC-800 Geiger counters use the [Rad Pro web installer](https://gissio.github.io/radpro-installer/).

## Device configuration

Use the following controls to operate your device:

* **Power on/off:** Press and hold the OK/Power key.
* **Switch measurement mode:** Use the Up/Back or Down/Settings key.
* **Switch secondary measurement view:** Press the OK/Power key.
* **Reset measurement/dismiss alert:** Press and long hold the Up/Back key.
* **Toggle pulse sound (measurement view only):** Press and long hold both the Up/Back and Down/Settings key.
* **Access settings:** Press and short hold the Down/Settings key.
* **Navigate options:** Use the Up/Back or Down/Settings key.
* **Select option:** Press and short hold the Right/Settings key or press the OK/Power key.
* **Go back:** Press and short hold the Up/Back key.
* **Toggle lock mode:** Press and long hold both the OK/Power and Up/Back keys.

To configure the device:

1. Go to **Settings > Geiger tube > Sensitivity** and select the option matching your Geiger-Müller tube.
2. For USB data connections on Windows, install the [CH340 driver](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523).

## Hardware-specific notes

<!-- Calculated as follows:

* With 1-byte differential values: [100 pages * (1 timestamp record/page [16 bytes] + 2024 differential records/page [1 byte each])] = 202500 records
* With 2-byte differential values: [100 pages * (1 timestamp record/page [16 bytes] + 1012 differential records/page [2 byte each])] = 101300 records

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 202,500 data points. At 20 cpm (normal radiation levels), this supports:
  * 4220 days at 60-minute intervals
  * 703 days at 10-minute intervals
  * 140 days at 1-minute intervals
  * 23 days at 10-second intervals
  * 56 hours at 1-second intervals

* **HV profile settings:**
  * Factory default: 2.5 kHz frequency, 16.3% duty cycle.
