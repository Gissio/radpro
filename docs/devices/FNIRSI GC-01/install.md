# Installing Rad Pro on the FNIRSI GC-01/JOY-IT JT-RAD01

This guide explains how to install **Rad Pro** on FNIRSI GC-01 and JOY-IT JT-RAD01 Geiger counters.

## What You'll Need

Make sure you have:

* A **USB data cable** (charging-only cables will not work)
* A **Windows 10 or Windows 11 computer**

> ⚠️ **Use a Windows 10/11 computer.** macOS and Linux are not supported for installation. If something goes wrong on an unsupported OS, recovery requires a Windows machine.

## Step 1: Download the Firmware

1. Go to the Rad Pro releases page: <https://github.com/Gissio/radpro/releases>
2. Download the latest `radpro-[version].zip`.
3. Extract the archive on your computer.

## Step 2: Flash Rad Pro

1. Make sure your device is powered off.
2. Connect it to your computer using the USB data cable.
3. A **USB drive** should appear.

   > If no drive appears, your device may be incompatible. See Troubleshooting below.

4. Open the extracted firmware folder: `fnirsi-gc01_[microprocessor]/firmware/` and choose the correct firmware file: `radpro-fnirsi-gc01_[microprocessor]-[language]-x.y.z-install.bin`
   * `[microprocessor]` is `apm32f103rbt6` or `ch32f103r8t6`. Start with `apm32f103rbt6`. If you get a **Drive full** error, disconnect the device and retry using `ch32f103r8t6`.
   * `[language]` is a two-letter code (e.g., `en`, `es`)
5. Copy the file to the USB drive.

The device will automatically reboot when the transfer completes.

After reboot, **Rad Pro is installed**.

### Troubleshooting

**Device not detected:**

* Make sure your USB cable supports data transfer
* Try another USB port
* Try a USB hub
* Hold **Right/Settings + OK/Power** to expose the USB drive
* After plugging in, press the **reset switch** (located near the USB connector, opposite the charge LED)

**Installation fails:**

* Hold **OK/Power** before plugging in the USB cable, and keep holding throughout the process
* If the file copy seems to succeed but the device doesn't reboot correctly, try copying the file a second time

**Want to restore the original firmware?**

* Copy the stock firmware from the [firmware](firmware) folder to the device using the same process

**Still no joy?**

* Try the alternative installation instructions: [install-stlink.md](install-stlink.md)

## Step 3: Basic Controls

Once installed, here are the essential controls:

### Navigation

* **Press Up / Down:** Change measurement mode / navigate menus
* **Press Right:** Select / open settings
* **Press Left:** Go back

### System

* **Hold OK:** Power on/off
* **Hold Left + OK:** Toggle lock mode

### Measurement Screen

* **Press Left:** Switch secondary display
* **Hold Left:** Reset measurement / dismiss alerts
* **Hold Right:** Toggle pulse sound
* **Press OK:** Turn off display

### Random generator

* **Press Right:** Restart random generator

## Step 4: First Configuration

Check that background radiation readings are in the expected range of **~0.05–0.25 µSv/h**.

If readings are too low:

1. Open **Settings > Geiger tube > Tube type**
2. Select **HH614**

## Step 5: Finish Up

* 📖 **Read the documentation**:
  * [Rad Pro user's manual](../../users-manual.md) – Easy guide to using Rad Pro
  * [Rad Pro reference manual](../../reference-manual.md) – Technical reference for Rad Pro
  * [Ionizing radiation field guide](https://github.com/Gissio/ionizing-radiation-field-guide) – Learn about ionizing radiation
* ⭐ **Support the project** by starring the repository: [https://github.com/Gissio/radpro](https://github.com/Gissio/radpro)
* 👥 **Help us grow** — share Rad Pro on social networks, forums, or with fellow enthusiasts!

## Hardware Notes

### CH32F103R8 (WCH) Devices

<!-- Calculated as follows:

* With 1-byte differential values: [5 pages * (1 timestamp record/page [10 bytes] + 1014 differential records/page [1 byte each])] = 5075 records
* With 2-byte differential values: [5 pages * (1 timestamp record/page [10 bytes] + 507 differential records/page [2 byte each])] = 2540 records

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

#### Data Storage Capacity

Rad Pro stores up to **5,075 measurements**.

At typical background radiation (~20 CPM), this corresponds to:

* **60-minute interval:** ~105 days
* **10-minute interval:** ~17 days
* **1-minute interval:** ~84 hours
* **10-second interval:** ~14 hours
* **1-second interval:** ~84 minutes

#### High Voltage (HV) Profiles

* **Factory default:**
  9.207 kHz frequency, 75% duty cycle

### APM32F103R8 (Geehy) Devices

<!-- Calculated as follows:

* With 1-byte differential values: [71 pages * (1 timestamp record/page [10 bytes] + 1014 differential records/page [1 byte each])] = 72065 records
* With 2-byte differential values: [71 pages * (1 timestamp record/page [10 bytes] + 507 differential records/page [2 byte each])] = 36068 records

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

#### Data Storage Capacity

Rad Pro stores up to **72,065 measurements**.

At typical background radiation (~20 CPM), this corresponds to:

* **60-minute interval:** ~1502 days
* **10-minute interval:** ~250 days
* **1-minute interval:** ~50 days
* **10-second interval:** ~8 days
* **1-second interval:** ~20 hours
