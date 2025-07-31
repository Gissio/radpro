# Installing Rad Pro on GQ GMC-800 Geiger counters

To install the Rad Pro firmware on GQ GMC-800 Geiger counters use the [Rad Pro Web Installer](https://gissio.github.io/radpro-installer/).

## Hardware-specific notes

<!-- Calculated as follows:

* With 1-byte differential values: [191 pages * (1 timestamp entry/page [10 bytes] + 1012 differential entries/page [1 byte each])] = 193483 entries
* With 2-byte differential values: [191 pages * (1 timestamp entry/page [10 bytes] + 506 differential entries/page [2 byte each])] = 96837 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 197,535 data points. At 20 cpm (normal radiation levels), this supports:
  * 4034 days at 60-minute intervals
  * 672 days at 10-minute intervals
  * 134 days at 1-minute intervals
  * 22 days at 10-second intervals
  * 53 hours at 1-second intervals

* **HV profile settings:**
  * Factory default: 2.5 kHz frequency, 16.3% duty cycle.
