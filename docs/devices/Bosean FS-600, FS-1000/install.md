# Installing Rad Pro on Bosean FS-600/FS-1000 Geiger counters

This guide explains how to install the Rad Pro firmware on Bosean FS-600 and FS-1000 Geiger counters.

To update the firmware, use the [Rad Pro web installer](https://gissio.github.io/radpro-installer/).

## What you'll need

* **ST-LINK V2 USB dongle** (or compatible clone): Available on [Amazon](https://www.amazon.com/s?k=st-link+v2).
* **4-pin header**: For the SWD connection.
* **Philips screwdriver:** For opening the device.
* **Optional:** Soldering iron and solder for a secure connection.
* **Windows users:** [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html)

## Step 1: Open the device

![Bosean FS-600 circuit board](img/fs600-board.jpg)

1. Power off the device and remove the batteries.
2. Unscrew the back case and carefully open the device.
3. Confirm the circuit board matches the image above (Geiger-Müller tube may vary). If it differs, you may have a different hardware revision, and Rad Pro may not work correctly. Report issues on the [Rad Pro GitHub issues page](https://github.com/Gissio/radpro/issues).

## Step 2: Connect the programmer

![Bosean FS-600 connectors](img/fs600-swd.jpg)

1. Optional: Solder a 4-pin header to the SWD pads on the board for a reliable connection.
2. Connect the ST-LINK V2 to the SWD pads using this pin configuration (left to right):
  * 3.3V
  * SWDIO
  * SWCLK
  * GND

![ST-LINK V2 programmer](../../img/ST-LINK-V2.png)

**WARNING:** Double-check connections to avoid damaging the device.

## Step 3: Flash the firmware

1. Windows users: install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
2. Download and extract the latest `radpro-flashtool-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
3. Navigate to the `bosean-[device]` folder and run the appropriate script:
  * Windows: Double click `install.bat`.
  * Linux: Run `install.sh` in a terminal.
  * macOS: Open `Terminal.app` (in `/Applications/Utilities`), navigate to the `bosean-[device]` folder, and drag `install.sh` onto the Terminal icon in the dock.
  * `[device]`: `fss600` or `fs1000`.
4. Select a language by entering its two-letter code (e.g., `en` for English) when prompted.
5. The installer automatically backs up the original firmware to the `backup` folder. Store this file securely to restore the original firmware if needed.
   * To restore, drag the backup file onto `install.bat` (Windows) or `install.sh` (macOS/Linux).
6. Reassemble the device after flashing.

## Step 4: Configure the device

### FS-600 controls

* **Power on/off:** Press and hold the OK/Power key.
* **Switch measurement mode:** Use the Up or Down key.
* **Switch secondary measurement view:** Press the Back key.
* **Reset measurement/dismiss alert:** Press and hold the Back key.
* **Toggle pulse sound (measurement view only):** Press and hold the Right/Settings key.
* **Sleep display (measurement view only):** Press the OK/Power key.
* **Access settings:** Press the Right/Settings key.
* **Navigate options:** Use the Up or Down key.
* **Select option:** Press the Right/Settings or OK/Power key.
* **Go back:** Prses the Back key.
* **Toggle lock mode:** Press and hold both the Back and OK/Power keys.

### FS-1000 controls

* **Power on/off:** Press and long hold the Set/Power key.
* **Switch measurement mode:** Press the Mode key.
* **Switch secondary measurement view:** Press the Set/Power key.
* **Reset measurement/dismiss alert:** Press and long hold the Mode key.
* **Access settings:** Press and short hold the Set/Power key.
* **Navigate options:** Use the Mode or Set/Power key.
* **Select option:** Press and short hold the Set/Power key.
* **Go back:** Press and short hold the Mode key.
* **Toggle lock mode:** Press and long hold both the Set/Power and Mode keys.

### Configuration steps

1. Go to **Geiger tube > HV Profile** and select:
  * **Energy-saving:** Lowest power use, best for background radiation.
  * **Accuracy:** Higher power use, ideal for elevated radiation levels.
  * **Factory default:** Highest power use, matches original firmware settings.
2. For USB data connections on Windows, install the [CH340 driver](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523).

For detailed usage, see the [Rad Pro User Manual](../../manual.md) and the [ionizing radiation field guide](https://github.com/Gissio/ionizing-radiation-field-guide).

## Step 5: Support Rad Pro

If you find Rad Pro useful:

* Watch the [Rad Pro GitHub repository](https://github.com/Gissio/radpro) for release updates.
* Star the project to show your support.

## Hardware notes

* **Pulse LED:** Not supported on all devices.

* **USB data connection:** Requires a USB data cable for computer connectivity.

<!-- Calculated as follows:

* With 1-byte differential values: [42 pages * (1 timestamp record/page [16 bytes] + 2024 differential records/page [1 byte each])] = 85050 records
* With 2-byte differential values: [42 pages * (1 timestamp record/page [16 bytes] + 1012 differential records/page [2 byte each])] = 42546 records

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 85,050 data points. At 20 cpm (normal radiation levels), this supports:
  * 1772 days at 60-minute intervals
  * 295 days at 10-minute intervals
  * 59 days at 1-minute intervals
  * 9 days at 10-second intervals
  * 23 hours at 1-second intervals

* **Voltage limitation:** Two Zener diodes cap the high voltage at 440 V (nominal).

* **HV profile settings:**
  * Factory default: 40 kHz frequency, 50% duty cycle.
  * Accuracy: 2.5 kHz frequency, 6.5% duty cycle.
  * Energy-saving: 2.5 kHz frequency, 3.0% duty cycle.
