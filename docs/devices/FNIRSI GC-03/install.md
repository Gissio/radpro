# Installing Rad Pro on the FNIRSI GC-03

This guide explains how to install **Rad Pro** on FNIRSI GC-03 Geiger counters.

## What You'll Need

Make sure you have:

* A **USB data cable** (charging-only cables will not work)
* A **computer** running Windows 10 or Windows 11

### Important Warning

⚠️ **DO NOT attempt installation from macOS or Linux.** These operating systems are **not supported** and can brick your device. You will need a Windows 10/11 computer to recover it if something goes wrong.

## Step 1: Download the Firmware

1. Go to the Rad Pro releases page: https://github.com/Gissio/radpro/releases
2. Download the latest `radpro-[version].zip`.
3. Extract it on your computer.

## Step 2: Enter Update Mode

1. Disconnect the device from USB.
2. Press and hold the **OK** button.
3. While holding it, power on the device.

If successful, the screen will display:

> **Update**

## Step 3: Flash Rad Pro

1. Connect the device to your computer.
2. A **USB drive** should appear.
3. Open the extracted firmware folder: `fnirsi-gc03/firmware/`
4. Choose the correct firmware file: `radpro-fnirsi-gc03-[language]-x.y.z-install.bin`
   * `[language]` is a two-letter code (e.g., `en`, `es`)
5. Copy the file to the USB drive.

The device will automatically reboot when the transfer completes.

After reboot, **Rad Pro is installed**.

### Troubleshooting

**Device does not reboot after copying:**

* Safely eject the drive and reconnect
* Try copying the file again

**Device not detected:**

* Check your USB cable (must support data)
* Try another USB port

**Want to go back to stock firmware?**

* Copy the original firmware (in the [firmware](firmware) folder) to the device using the same process

## Step 4: Basic Controls

Once installed, here are the essential controls:

### Navigation

* **Rotate knob:** Change measurement mode / navigate menus
* **Press OK:** Select / open settings
* **Press Back:** Go back

### System

* **Hold Power:** Power on/off
* **Press Power:** Turn off display
* **Hold Back + Power:** Toggle lock mode

### Measurement Screen

* **Press Back:** Switch secondary display
* **Hold Back:** Reset measurement / dismiss alerts
* **Hold OK:** Toggle pulse sound

### Random generator

* **Press OK:** Restart random generator

## Step 5: Finish Up

* 📖 **Read the documentation**:
  * [Rad Pro user's manual](../../users-manual.md) – Easy guide to using Rad Pro
  * [Rad Pro reference manual](../../reference-manual.md) – Technical reference for Rad Pro
  * [Ionizing radiation field guide](https://github.com/Gissio/ionizing-radiation-field-guide) – Learn about ionizing radiation
* ⭐ **Support the project** by starring the repository: [https://github.com/Gissio/radpro](https://github.com/Gissio/radpro)
* 👥 **Help us grow** — share Rad Pro on social networks, forums, or with fellow enthusiasts!

## Hardware Notes

<!-- Calculated as follows:

* With 1-byte differential values: [42 pages * (1 timestamp record/page [10 bytes] + 4086 differential records/page [1 byte each])] = 171654 records
* With 2-byte differential values: [42 pages * (1 timestamp record/page [10 bytes] + 2043 differential records/page [2 byte each])] = 85188 records

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

### Data Storage Capacity

Rad Pro stores up to **171,654 measurements**.

At typical background radiation (~20 CPM), this corresponds to:

* **60-minute interval:** ~3577 days
* **10-minute interval:** ~596 days
* **1-minute interval:** ~119 days
* **10-second interval:** ~19 days
* **1-second interval:** ~47 hours
