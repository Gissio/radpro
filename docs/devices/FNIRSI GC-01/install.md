# Installing Rad Pro on FNIRSI GC-01/JOY-IT JT-RAD01 Geiger counters

This guide explains how to install the Rad Pro firmware on FNIRSI GC-01 and JOY-IT JT-RAD01 Geiger counters.

## What you'll need

To install Rad Pro, you'll need:

* **USB data cable:** Must support data transfer, not just charging.
* **Philips screwdriver** to open the device.

## Step 1: Open the device

![FNIRSI GC-01 circuit board types](img/gc-01-board-type.jpg)

1. Power off the device.
2. Unscrew the back case and carefully open the device.
3. Identify the microprocessor type (square chip), which should be CH32F103R8T6 (WCH) or APM32F103RBT6 (Geehy). **Warning:** Rad Pro is not compatible with devices using unmarked chips.
4. Identify the Geiger-Müller tube (marked on the glass as J305, J321, J613, J614, or M4011). For unmarked tubes:
   * 55 mm tubes: Assume J614.
   * 65 mm tubes: Assume J613.
5. Reassemble the device.

## Step 2: Flash the firmware

1. Download and extract the latest `radpro-fnirsi-gc01_[microprocessor]-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
2. Choose the correct firmware file: `radpro-fnirsi-gc01_[microprocessor]-[language]-x.y.z-install.bin`.
   * `[microprocessor]`: `ch32f103r8t6` or `apm32f103rbt6`.
   * `[language]` Two-letter code for your preferred language (e.g., `en` for English).
3. Connect the device to a **Windows computer** using a **USB 2.0 port** and the USB data cable. **Warning:** Installation may fail on macOS, Linux, or USB 3.0 ports.
4. Power on the device. A USB drive should appear on your computer.
5. Copy the selected firmware file to the USB drive. The device should restart automatically, installing Rad Pro.

**Note:**

* Some devices require holding the **Power** button from power-on until the firmware is copied and the device restarts.
* Others may need the **Right/Settings** and **OK/Power** keys pressed together to make the USB drive available.

**Troubleshooting:**

* If the installation fails, press the **reset switch** next to the USB connector and repeat the process.
* To restore the original firmware, copy the [original firmware](firmware) to the USB drive.
* For persistent issues, follow the [FNIRSI GC-01 alternative installation instructions](install-stlink.md).

## Step 3: Configure the device

Use the following controls to operate your device:

  * **Power on/off:** Press and hold the OK/Power key.
  * **Switch measurement mode:** Use the Up or Down key.
  * **Switch secondary measurement view:** Press the Left/Back key.
  * **Reset measurement/dismiss alarm:** Press and hold the Left/Back key.
  * **Toggle pulse sound (measurement view only):** Press and hold the Down key.
  * **Sleep display (measurement view only):** Press the OK/Power key.
  * **Access settings:** Press the Right/Settings key.
  * **Navigate options:** Use the Up or Down key.
  * **Select option:** Press the OK/Power or Right/Settings key.
  * **Go back:** Press the Left/Back key.
  * **Toggle lock mode:** Press and hold both the Left/Back and OK/Power keys.

To configure the device:

1. Go to **Settings > Geiger tube > Sensitivity** and select the option that matches your Geiger-Müller tube.
2. Go to **Geiger tube > HV Profile** and select:
  * **Energy-saving:** Lowest power use, best for background radiation.
  * **Accuracy:** Higher power use, ideal for elevated radiation levels.
  * **Factory default:** Highest power use, matches original firmware settings.
3. If the device registers unintended key presses, select the **Energy-saving** HV profile or configure a custom HV profile to resolve the issue.

## Step 4: Support Rad Pro

If you find Rad Pro useful:

* Watch the [Rad Pro GitHub repository](https://github.com/Gissio/radpro) for release updates.
* Star the project to show your support.

## Hardware-specific notes

### FNIRSI GC-01/JOY-IT JT-RAD01 with CH32F103R8 (WCH) microprocessor

* Enable **Data Mode** in the settings to establish a USB data connection. Disable when not in use to save power.

<!-- Calculated as follows:

* With 1-byte differential values: [5 pages * (1 timestamp entry/page [10 bytes] + 1012 differential entries/page [1 byte each])] = 5065 entries
* With 2-byte differential values: [5 pages * (1 timestamp entry/page [10 bytes] + 506 differential entries/page [2 byte each])] = 2535 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 6,078 data points. At 20 cpm (normal radiation levels), this supports:
  * 105 days at 60-minute intervals
  * 17 days at 10-minute intervals
  * 84 hours at 1-minute intervals
  * 14 hours at 10-second intervals
  * 84 minutes at 1-second intervals

* **HV profile settings:**
  * Factory default: 9.207 kHz frequency, 75% duty cycle.
  * Energy-saving: 5 kHz frequency, 1.5% duty cycle.

### FNIRSI GC-01/JOY-IT JT-RAD01 with APM32F103R8 (Geehy) processor

* The USB data connection is always available.

<!-- Calculated as follows:

* With 1-byte differential values: [67 pages * (1 timestamp entry/page [10 bytes] + 1012 differential entries/page [1 byte each])] = 67871 entries
* With 2-byte differential values: [67 pages * (1 timestamp entry/page [10 bytes] + 506 differential entries/page [2 byte each])] = 33969 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 67,871 data points. At 20 cpm (normal radiation levels), this supports:
  * 1415 days at 60-minute intervals
  * 235 days at 10-minute intervals
  * 47 days at 1-minute intervals
  * 7 days at 10-second intervals
  * 18 hours at 1-second intervals

* **HV profile settings:**
  * Factory default: 47.058 kHz frequency, 50% duty cycle.
  * Energy-saving: 5 kHz frequency, 1.5% duty cycle.
