# Installing Rad Pro on Bosean FS-600/FS-1000 Geiger counters

This guide explains how to install the Rad Pro firmware on Bosean FS-600 and FS-1000 Geiger counters.

## Required supplies

To install Rad Pro, you'll need:

* [ST-LINK V2 USB dongle (or compatible clone)](https://www.amazon.com/s?k=st-link+v2)
* 4-pin header
* Philips screwdriver
* Optional: Soldering iron and solder
* For Windows users: [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html)

## Step 1: Open the device

![Bosean FS-600 circuit board](img/fs600-board.jpg)

1. Power off the device and remove the batteries.
2. Unscrew the back case and carefully open the device.
3. Verify that the circuit board matches the image above (the Geiger-Müller tube may vary). If the board differs, you may have a different hardware revision, and Rad Pro may not function correctly. If issues arise, report them on the [Rad Pro GitHub issues page](https://github.com/Gissio/radpro/issues).

## Step 2: Connect the programmer

![Bosean FS-600 connectors](img/fs600-swd.jpg)

1. Optional: Solder a 4-pin header to the SWD pads on the board for a secure connection.
3. Connect the ST-LINK V2 to the SWD pads using the following pin configuration (left to right):
  * 3.3V
  * SWDIO
  * SWCLK
  * GND

![ST-LINK V2 programmer](../../img/ST-LINK-V2.png)

**WARNING:** Double-check electrical connections to prevent damage to the device.

## Step 3: Flash the firmware

1. For Windows users, install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
2. Download and extract the latest `radpro-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
3. Navigate to the `bosean` folder and run the appropriate script based on your operating system:
  * Windows: Double click `bosean-[device]-[type].bat`.
  * Linux: Run `bosean-[device]-[type].sh` in a terminal.
  * macOS: Open `Terminal.app` (in `/Applications/Utilities`), navigate to the `bosean` folder, and drag `bosean-[device]-[type].sh` onto the Terminal icon in the dock.
  * `[device]`: Type of device (`fss600` or `fs1000`).
  * `[type]`: Installation type (`install` or `update`). Note: Not all releases support updating.
4. The installer will prompt you to select a language using two-letter language codes. Enter the desired code to proceed.
5. The installer automatically backs up the original firmware to the `backup` folder. Store this securely to restore the original firmware if needed. To restore, drag the backup file onto `bosean-[device]-[type]-install.bat` (Windows) or `bosean-[device]-[type]-install.sh` (macOS/Linux).
6. Close and reassemble the device.

## Step 4: Configure the device

### FS-600 controls

  * **Power on/off:** Press and hold the OK/Power key.
  * **Switch measurement mode:** Use the Up or Down key.
  * **Switch secondary measurement view:** Press the Back key.
  * **Reset measurement/dismiss alarm:** Press and hold the Back key.
  * **Toggle pulse sound (measurement view only):** Press and hold the Down key.
  * **Sleep display (measurement view only):** Press the OK/Power key.
  * **Access settings:** Press the Right/Settings key.
  * **Navigate options:** Use the Up or Down key.
  * **Select option:** Press the Right/Settings or OK/Power key.
  * **Go back:** Prses the Back key.
  * **Toggle lock mode:** Press and hold both the OK/Power and Back keys.

### FS-1000 controls

  * **Power on/off:** Press and long hold the Set/Power key.
  * **Switch measurement mode:** Press the Mode key.
  * **Switch secondary measurement view:** Press the Set/Power key.
  * **Reset measurement/dismiss alarm:** Press and long hold the Mode key.
  * **Access settings:** Press and short hold the Set/Power key.
  * **Navigate options:** Use the Mode or Set/Power key.
  * **Select option:** Press and short hold the Set/Power key.
  * **Go back:** Press and short hold the Mode key.
  * **Toggle lock mode:** Press and long hold both the Set/Power and Mode keys.

### Configuration steps

1. In **Geiger tube > HV Profile**, choose an appropriate profile:
  * **Energy-saving:** Lowest power consumption, ideal for background radiation level.
  * **Accuracy:** Higher power consumption, suitable for elevated radiation levels.
  * **Factory default:** Highest power consumption, uses the original firmware profile.
2. For USB data connections under Windows, install the [CH340 driver](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523).

For detailed usage instructions, refer to the [Rad Pro User Manual](../../manual.md) and the [ionizing radiation field guide](../../field-guide/field-guide.md).

## Step 5: Final steps

If you find Rad Pro useful, consider:

* Watching the [Rad Pro GitHub repository](https://github.com/Gissio/radpro) for release notifications.
* Starring the project to show your support.

## Hardware-specific notes

* **Pulse LED:** Not supported on all devices.

* **USB data connection:** Use a USB data cable to connect to a computer.

<!-- Calculated as follows:

* With 1-byte differential values: [43 pages * (1 timestamp entry/page [16 bytes] + 2024 differential entries/page [1 byte each])] = 87075 entries
* With 2-byte differential values: [43 pages * (1 timestamp entry/page [16 bytes] + 1012 differential entries/page [2 byte each])] = 43559 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 89,100 data points. At 20 cpm (normal radiation levels), this supports:
  * 1814 days at 60-minute intervals
  * 302 days at 10-minute intervals
  * 60 days at 1-minute intervals
  * 10 days at 10-second intervals
  * 24 hours at 1-second intervals

* **Voltage limitation:** Two Zener diodes limit the maximum voltage to 440 V (nominal).

* **HV profile settings:**
  * Factory default: 40 kHz frequency, 50% duty cycle.
  * Accuracy: 2.5 kHz frequency, 6.5% duty cycle.
  * Energy-saving: 2.5 kHz frequency, 3.0% duty cycle.
