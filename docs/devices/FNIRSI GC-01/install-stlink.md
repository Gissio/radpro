# Installing Rad Pro on FNIRSI GC-01/JOY-IT JT-RAD01 Geiger counters with an ST-LINK

This guide explains how to install the Rad Pro firmware on an FNIRSI GC-01 or JOY-IT JT-RAD01 Geiger counter with a CH32F103R8T6 (WCH) or APM32F103RBT6 (Geehy) microprocessor. Use this method if:

* You cannot access the USB drive as described in the [regular installation instructions](install.md).
* Your device is unresponsive and requires recovery.

## Required supplies

To install Rad Pro, you'll need:

* [ST-LINK V2 USB dongle (or compatible clone)](https://www.amazon.com/s?k=st-link+v2)
* 4-pin header
* Philips screwdriver
* Optional: Soldering iron and solder
* For Windows users: [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html)

## Step 1: Open the device

![FNIRSI GC-01 circuit board types](img/gc-01-board-type.jpg)

1. Power off the device.
2. Unscrew the back case and carefully open the device.
3. Note the microprocessor type (square chip), which should be CH32F103R8T6 (WCH) or APM32F103RBT6 (Geehy). The picture above should help you identify the MCU. **WARNING:** Rad Pro is not compatible with devices using a CACHIP microprocessor.
4. Note the Geiger-Müller tube type (marked on the glass as J614, J321, J305 or M4011). For unmarked tubes measuring 55 or 65 mm, assume J614 or J613, respectively.

## Step 2: Connect the programmer

![FNIRSI GC-01 connectors](img/gc-01-swd.jpg)

Now, follow these steps:

* Optional: Solder a 4-pin header to the JP1 pads on the board for a secure connection.
* Connect the ST-LINK V2 to the four pads next to the button cell battery.
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

**WARNING:** Double-check electrical connections to prevent damage to the device.

## Step 3: Flash the firmware

1. For Windows users, install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
2. Download and extract the latest `radpro-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
3. Navigate to the `fnirsi` folder and run the appropriate script based on your operating system:
  * Windows: Double click `fnirsi-gc01_[mcu]-[type].bat`.
  * Linux: Run `fnirsi-gc01_[mcu]-[type].sh` in a terminal.
  * macOS: Open `Terminal.app` (in `/Applications/Utilities`), navigate to the `fnirsi` folder, and drag `fnirsi-gc01_[mcu]-[type].sh` onto the Terminal icon in the dock.
  * `[mcu]`: The microprocessor type of your board (`ch32f103r8t6` or `apm32f103rbt6`).
  * `[type]`: Installation type (`install` or `update`). Note: Not all releases support updating.
4. The installer will prompt you to select a language using two-letter language codes. Enter the desired code to proceed.
5. If flashing fails, try powering the device via USB and repeat the process.
6. The installer automatically backs up the original firmware to the `backup` folder. Store this securely to restore the original firmware if needed. To restore, drag the backup file onto `fnirsi-gc01_[mcu]-[type]-install.bat` (Windows) or `fnirsi-gc01_[mcu]-[type]-install.sh` (macOS/Linux).
7. Close and reassemble the device.
8. Proceed to [step 3 of the regular installation instructions](install.md#step-3-configure-the-device).
