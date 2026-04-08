# Installing Rad Pro on FNIRSI GC-03 Geiger counters

This guide explains how to install the Rad Pro firmware on FNIRSI GC-03 Geiger counters.

## What you'll need

* **USB data cable:** Must support data transfer, not just charging.

## Step 1: Flash the firmware

1. Download and extract the latest `radpro-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
2. Disconnect the device from USB.
3. Power on the device while holding the "OK" button. The screen should show **Update**.
4. Connect the device to your computer. A USB drive should appear.
5. Navigate to the `fnirsi-gc03/install` folder. Select the appropriate firmware file: `radpro-fnirsi-gc03-[language]-x.y.z-install.bin`.
   * `[language]` Two-letter code for your preferred language (e.g., `en` for English).
6. Copy the selected firmware file to the USB drive. The device should restart automatically with Rad Pro installed.

**Linux users:**

The USB storage mode of the GC-03 boot loader is fragile. Mounting the GC-03 using `mount` or `pmount` seems to be more robust than using file explorers based on [GVfs](https://en.wikipedia.org/wiki/GVfs). Sometimes it helps to copy the file twice, with a `sync` call in between. When the GC-03 powers off and shows a white screen, the update was accepted.

**Troubleshooting:**

* To restore the original firmware, copy the [original firmware](firmware) to the USB drive.

## Step 2: Configure the device

Use the following controls to operate your device:

* **Power on/off:** Press and hold the Power key.
* **Switch measurement mode:** Use the rotary knob.
* **Switch secondary measurement view:** Press the Back key.
* **Reset measurement/dismiss alert:** Press and hold the Back key.
* **Toggle pulse sound (measurement view only):** Press and hold the OK/Settings key.
* **Sleep display (measurement view only):** Press the Power key.
* **Access settings:** Press the OK/Settings key.
* **Navigate options:** Use the rotary knob.
* **Select option:** Press the OK/Settings key.
* **Go back:** Press the Back key.
* **Toggle lock mode:** Press and hold both the Back and Power keys.
* **Restart random generator:** Press the OK/Settings key.

## Step 3: Support Rad Pro

If you find Rad Pro useful:

* Watch the [Rad Pro GitHub repository](https://github.com/Gissio/radpro) for release updates.
* Star the project to show your support.

## Hardware-specific notes

<!-- Calculated as follows:

* With 1-byte differential values: [73 pages * (1 timestamp record/page [10 bytes] + 1012 differential records/page [1 byte each])] = 73949 records
* With 2-byte differential values: [73 pages * (1 timestamp record/page [10 bytes] + 506 differential records/page [2 byte each])] = 37011 records

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 67,871 data points. At 20 cpm (normal radiation levels), this supports:
  * 1542 days at 60-minute intervals
  * 257 days at 10-minute intervals
  * 51 days at 1-minute intervals
  * 8 days at 10-second intervals
  * 20 hours at 1-second intervals

* **HV profile settings:**
  * Factory default: 47.058 kHz frequency, 50% duty cycle.
  * Energy-saving: 5 kHz frequency, 1.5% duty cycle.
