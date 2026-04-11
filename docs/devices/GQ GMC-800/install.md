# Installing Rad Pro on the GQ GMC-800

This guide explains how to install **Rad Pro** on GQ GMC-800 Geiger counters.

## What You'll Need

Make sure you have:

* A **USB data cable** (charging-only cables will not work)
* A **computer** with a modern web browser (Chrome/Edge recommended)

Windows only:

* Install the CH340 driver:
  [https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523)

## Step 1: Installation

1. Open the Rad Pro web installer:
   [https://gissio.github.io/radpro-installer/](https://gissio.github.io/radpro-installer/)

2. Connect your device via USB.

3. Follow the on-screen instructions:

   * Select your device
   * Choose your preferred language
   * Start the installation

The device will reboot automatically when installation completes.

### Restoring the Original Firmware

To revert to the stock firmware:

* Contact GQ Electronics at:
  **[support@gqelectronicsllc.com](mailto:support@gqelectronicsllc.com)**

Include:

* Device model
* Serial number
* Request for the latest firmware update

## Step 2: Basic Controls

Once installed, here are the essential controls:

### Navigation

* **Press Up / Down:** Change measurement mode / navigate menus
* **Press Power:** Select / open settings
* **Press Back:** Go back

### System

* **Hold Power:** Power on/off
* **Hold Back + Power:** Toggle lock mode

### Measurement Screen

* **Press Back:** Switch secondary display
* **Hold Back:** Reset measurement / dismiss alerts
* **Hold Up:** Voice play current measurement
* **Hold Down:** Toggle pulse sound

### Random generator

* **Press Power:** Restart random generator

## Step 3: Finish Up

* 📖 **Read the documentation**:
  * [Rad Pro user's manual](../../users-manual.md) – Easy guide to using Rad Pro
  * [Rad Pro reference manual](../../reference-manual.md) – Technical reference for Rad Pro
  * [Ionizing radiation field guide](https://github.com/Gissio/ionizing-radiation-field-guide) – Learn about ionizing radiation
* ⭐ **Support the project** by starring the repository: [https://github.com/Gissio/radpro](https://github.com/Gissio/radpro)
* 👥 **Help us grow** — share Rad Pro on social networks, forums, or with fellow enthusiasts!

## Hardware Notes

<!-- Calculated as follows:

* With 1-byte differential values: [101 pages * (1 timestamp record/page [16 bytes] + 2032 differential records/page [1 byte each])] = 205333 records
* With 2-byte differential values: [101 pages * (1 timestamp record/page [16 bytes] + 1016 differential records/page [2 byte each])] = 102717 records

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

### Data Storage Capacity

Rad Pro stores up to **205,333 measurements**.

At typical background radiation (~20 CPM), this corresponds to:

* **60-minute interval:** ~4279 days
* **10-minute interval:** ~713 days
* **1-minute interval:** ~142 days
* **10-second interval:** ~23 days
* **1-second interval:** ~57 hours

### High Voltage (HV) Profiles

* **Factory default:**
  2.5 kHz frequency, 16.3% duty cycle
