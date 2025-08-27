# Installing Rad Pro on FNIRSI GC-01/JOY-IT JT-RAD01 Geiger counters with an ST-LINK

This guide explains how to install the Rad Pro firmware on an FNIRSI GC-01 or JOY-IT JT-RAD01 Geiger counter with a CH32F103R8T6 (WCH) or APM32F103RBT6 (Geehy) microprocessor. Use this method if:

* You cannot access the USB drive as described in the [regular installation instructions](install.md).
* Your device is unresponsive and requires recovery.

## What you'll need

* **ST-LINK V2 USB dongle** (or compatible clone): Available on [Amazon](https://www.amazon.com/s?k=st-link+v2).
* **4-pin header**: For the SWD connection.
* **Philips screwdriver:** For opening the device.
* **Optional:** Soldering iron and solder for a secure connection.
* **Windows users:** [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html)

## Step 1: Open the device

![FNIRSI GC-01 circuit board types](img/gc-01-board-type.jpg)

1. Power off the device.
2. Unscrew the back case and carefully open the device.
3. Identify the microprocessor type (square chip), which should be CH32F103R8T6 (WCH) or APM32F103RBT6 (Geehy). **Warning:** Rad Pro is not compatible with devices using unmarked chips.
4. Identify the Geiger-Müller tube, marked as **J305**, **J321**, **J613**, **J614**, or **M4011**. For unmarked tubes:
   * **55 mm tubes:** Assume J614.
   * **65 mm tubes:** Assume J613.
5. Reassemble the device.

## Step 2: Connect the programmer

![FNIRSI GC-01 connectors](img/gc-01-swd.jpg)

Now, follow these steps:

1. Optional: Solder a 4-pin header to the JP1 pads on the board for a reliable connection.
2. Connect the ST-LINK V2 to the four pads next to the button cell battery.
  * If the pads are unlabeled (left to right):
    * 3.3V
    * SWDIO
    * SWCLK
    * GND

  * If the pads are labeled:
    * CLK → SWCLK
    * DIO → SWDIO
    * GND → GND
    * VCC → 3.3V

![ST-LINK V2 programmer](../../img/ST-LINK-V2.png)

**WARNING:** Double-check connections to avoid damaging the device.

## Step 3: Flash the firmware

1. Windows users: install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
2. Download and extract the latest `radpro-flashtool-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
3. Navigate to the `fnirsi-gc01_[microprocessor]` folder and run the appropriate script:
  * Windows: Double click `install.bat`.
  * Linux: Run `install.sh` in a terminal.
  * macOS: Open `Terminal.app` (in `/Applications/Utilities`), navigate to the `fnirsi-gc01_[microprocessor]` folder, and drag `install.sh` onto the Terminal icon in the dock.
  * `[microprocessor]`: `stm32f051c8`, `gd32f150c8`, or `gd32f103c8`.
4. Select a language by entering its two-letter code (e.g., `en` for English) when prompted.
5. If flashing fails, try powering the device via USB and repeat the process.
6. Reassemble the device after flashing.
7. Proceed to [step 3 of the regular installation instructions](install.md#step-3-configure-the-device).
