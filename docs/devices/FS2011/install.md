# Installing Rad Pro on FS2011/YT-203B Geiger counters

This guide explains how to install the Rad Pro firmware on FS2011 and YT-203B Geiger counters.

## What you'll need

* **ST-LINK V2 USB dongle** (or compatible clone): Available on [Amazon](https://www.amazon.com/s?k=st-link+v2).
* **4-pin header**: For the SWD connection.
* **Philips screwdriver:** For opening the device.
* **Optional:** Soldering iron and solder for a secure connection.
* **Windows users:** [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html)

## Step 1: Open the device

![FS2011 circuit board types](img/fs2011-board-type.jpg)

1. Power off the device and remove the batteries.
2. Unscrew the back case and carefully open the device.
3. Confirm the circuit board matches the image above (Geiger-Müller tube may vary). If it differs, you may have a different hardware revision, and Rad Pro may not work correctly. Report issues on the [Rad Pro GitHub issues page](https://github.com/Gissio/radpro/issues).
4. Identify the microprocessor (square chip), either **STM32F051C8**, **GD32F150C8**, or **GD32F103C8**.
4. Identify the Geiger-Müller tube, marked as **J305**, **J321**, **J613**, **J614**, or **M4011**. For unmarked tubes:
   * **55 mm tubes:** Assume J614.
   * **65 mm tubes:** Assume J613.

## Step 2: Connect the programmer

![FS2011 connectors](img/fs2011-swd.jpg)

1. Optional: Solder a 4-pin header to the XS1 pads on the board for a reliable connection.
2. Connect the ST-LINK V2 to the XS1 pads using this pin configuration (top to bottom):
  * GND
  * SWCLK
  * SWDIO
  * 3.3V

![ST-LINK V2 programmer](../../img/ST-LINK-V2.png)

**WARNING:** Double-check connections to avoid damaging the device.

## Step 3: Flash the firmware

1. Windows users: install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
2. Download and extract the latest `radpro-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
3. Navigate to the `fs2011-[microprocessor]` folder and run the appropriate script:
  * Windows: Double click `install.bat`.
  * Linux: Run `install.sh` in a terminal.
  * macOS: Open `Terminal.app` (in `/Applications/Utilities`), navigate to the `fs2011-[microprocessor]` folder, and drag `install.sh` onto the Terminal icon in the dock.
  * `[microprocessor]`: `stm32f051c8`, `gd32f150c8`, or `gd32f103c8`.
4. Select a language by entering its two-letter code (e.g., `en` for English) when prompted.
5. The installer automatically backs up the original firmware to the `backup` folder. Store this file securely to restore the original firmware if needed.
   * To restore, drag the backup file onto `install.bat` (Windows) or `install.sh` (macOS/Linux).
6. Reassemble the device after flashing.

## Step 4: Configure the device

Use the following controls to operate your device:

* **Power on/off:** Press and hold the Power key.
* **Switch measurement mode:** Use the Up or Down key.
* **Switch secondary measurement view:** Press the Play/Pause key.
* **Reset measurement/dismiss alert:** Press and hold the Play/Pause key.
* **Toggle pulse sound (measurement view only):** Press and hold the Menu/OK key.
* **Sleep display (measurement view only):** Press the Power key.
* **Access settings:** Press the Menu/OK key.
* **Navigate options:** Use the Up or Down key.
* **Select option:** Press the Menu/OK or Power key.
* **Go back:** Prses the Play/Pause key.
* **Toggle lock mode:** Press and hold both the Play/Pause and Power keys.

To configure the device:

1. Go to **Settings > Geiger tube > Sensitivity** and select the option that matches your Geiger-Müller tube.
2. Go to **Geiger tube > HV Profile** and select:
  * **Energy-saving:** Lowest power use, best for background radiation.
  * **Accuracy:** Higher power use, ideal for elevated radiation levels.
  * **Factory default:** Highest power use, matches original firmware settings.

For detailed usage, see the [Rad Pro User Manual](../../manual.md) and the [ionizing radiation field guide](https://github.com/Gissio/ionizing-radiation-field-guide).

## Step 5: Optional modifications

![FS2011 FTDI connection](img/fs2011-ftdi.jpg)

Enhance your device with these optional modifications:

* **Add USB connectivity:**
  * Required: [3.3 V USB serial converter](https://www.amazon.com/s?k=ftdi+board) and [wire-wrap](https://www.amazon.com/s?k=wirewrap+30).
  * **WARNING:** Do not use batteries when powering via USB.
  * Steps:
    1. Configure the serial converter for 3.3 V.
    2. Connect GND and 5V pins to the FS2011’s 0V and 5V pads, respectively.
    3. Connect the microprocessor’s PA2 pin to the converter’s RX pin.
    4. Connect the microprocessor’s PA3 pin to the converter’s TX pin.
* **Add vibration feedback:** Solder a vibration motor (e.g. from an old cellphone) with one terminal to the Q5 transistor output and the other to 3.3 V.
* **Improve XS1 access:** File the battery holder clip hole to fit a 4-pin header connector. Solder four cables between XS1 and the header, then secure the header inside the back case with a glue gun for access without fully opening the device.
* **Align Geiger tube:** If the tube is misaligned with the back case holes, use a heat gun or glue gun to reposition it. Handle the delicate glass tube with care.
* **Fix battery contact issues:** For AA rechargeable batteries with low-profile caps, apply solder to the battery holder’s pads to ensure proper contact.
* **Increase buzzer volume:** Drill holes in the back case in front of the buzzer.

## Step 5: Support Rad Pro

If you find Rad Pro useful:

* Watch the [Rad Pro GitHub repository](https://github.com/Gissio/radpro) for release updates.
* Star the project to show your support.

## Hardware-specific notes

<!-- Calculated as follows:

* With 1-byte differential values: [21 pages * (1 timestamp record/page [10 bytes] + 1012 differential records/page [1 byte each])] = 21273 records
* With 2-byte differential values: [21 pages * (1 timestamp record/page [10 bytes] + 506 differential records/page [2 byte each])] = 10647 enrecordtries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 23,299 data points. At 20 cpm (normal radiation levels), this supports:
  * 443 days at 60-minute intervals
  * 73 days at 10-minute intervals
  * 14 days at 1-minute intervals
  * 59 hours at 10-second intervals
  * 5 hours at 1-second intervals

* **Voltage limitation:** Two Zener diodes limit the maximum voltage to 440 V (nominal).

* **HV profile settings:**
  * Factory default: 40 kHz frequency, 50% duty cycle.
  * Accuracy: 2.5 kHz frequency, 6.5% duty cycle.
  * Energy-saving: 2.5 kHz frequency, 3.0% duty cycle.
