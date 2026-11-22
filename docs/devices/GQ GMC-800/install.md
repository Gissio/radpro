# Installing Rad Pro on the GQ GMC-800

This guide explains how to install **Rad Pro** on GQ GMC-800 and GMC-800V2 Geiger counters.

## What You'll Need

* A **USB data cable** (charging-only cables will not work)
* A **computer** with a modern web browser (Chrome/Edge recommended)

## Step 1: Identify Your Device

On your device, open **Menu > About** and check the **Revision**:

* **Below Re2.00:** you have a GMC-800. Follow [Installing on the GMC-800](#installing-on-the-gmc-800).
* **Re2.00 or higher:** you have a GMC-800V2. Follow [Installing on the GMC-800V2](#installing-on-the-gmc-800v2).

## Step 2: Installation

### Installing on the GMC-800

1. Windows only: install the CH340 driver:
   [https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523)

2. Open the Rad Pro web installer:
   [https://gissio.github.io/radpro-installer/](https://gissio.github.io/radpro-installer/)

3. Connect your device via USB.

4. Follow the on-screen instructions:

   * Select your device
   * Choose your preferred language
   * Start the installation

The device will reboot automatically when installation completes.

### Installing on the GMC-800V2

1. **Download the firmware:**
   * Go to the Rad Pro releases page: <https://github.com/Gissio/radpro/releases>
   * Download the latest `radpro-[version].zip`.
   * Extract the archive on your computer.

2. **Enter update mode:**
   * Turn the device off and disconnect it from USB.
   * Press and hold the **Back** button while connecting the device to your computer.
   * A USB drive named **BOOTLOADER** appears, containing a `firmware.bin` file.

3. **Back up the original firmware:**
   * Copy `firmware.bin` from the BOOTLOADER drive to a safe place on your computer.

   > ⚠️ **Do not skip this step.** This file is your only backup of the stock firmware, and you need it to restore your device.

4. **Copy Rad Pro:**
   * Open the extracted firmware folder: `gq-gmc800v2/firmware/`
   * Choose the firmware file: `radpro-gq-gmc800v2-[language]-x.y.z.bin`
     * `[language]` is a two-letter code (e.g., `en`, `es`)
   * Copy the file to the BOOTLOADER drive.

The device will restart with Rad Pro when the transfer completes.

**Troubleshooting:**

* **Device not detected:** make sure your USB cable supports data transfer, try another USB port, or try a USB hub.
* **Installation fails or the screen stays dark:** enter update mode again and copy the file again, or restore the original firmware (see below).

### Restoring the Original Firmware

**GMC-800:** contact GQ Electronics at **[support@gqelectronicsllc.com](mailto:support@gqelectronicsllc.com)** and include:

* Device model
* Serial number
* Request for the latest firmware update

**GMC-800V2:** enter update mode as described above, then copy the `firmware.bin` you backed up to the BOOTLOADER drive.

## Step 3: Basic Controls

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

## Step 4: Finish Up

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
