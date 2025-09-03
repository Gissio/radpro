# Installing Rad Pro on FNIRSI GC-01/JOY-IT JT-RAD01 Geiger counters

This guide explains how to install the Rad Pro firmware on FNIRSI GC-01 and JOY-IT JT-RAD01 Geiger counters.

## What you'll need

* **USB data cable:** Must support data transfer, not just charging.
* **Philips screwdriver:** For opening the device.

## Step 1: Open the device

![FNIRSI GC-01 circuit board types](img/gc-01-board-type.jpg)

1. Power off the device.
2. Unscrew the back case and carefully open the device.
3. Identify the microprocessor (square chip): either **CH32F103R8T6 (WCH)** or **APM32F103RBT6 (Geehy)**. If unmarked, proceed to the next step.
4. Identify the Geiger-Müller tube, marked as **J305**, **J321**, **J613**, **J614**, or **M4011**. For unmarked tubes:
   * **55 mm tubes:** Assume J614.
   * **65 mm tubes:** Assume J613.
5. Reassemble the device.

## Step 2: Flash the firmware

1. Download and extract the latest `radpro-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
2. Connect the device to a **Windows computer** using a **USB 2.0 port** and the USB data cable. **Note:** Installation may fail on macOS, Linux, or USB 3.0 ports.
3. Power on the device. A USB drive should appear on your computer. If it does not, the device may be incompatible with Rad Pro.
4. If you identified the microprocessor, navigate to the `fnirsi-gc01_[micropreocessor]/install` folder. Select the appropriate firmware file: `radpro-fnirsi-gc01_[microprocessor]-[language]-x.y.z-install.bin`.
   * `[microprocessor]`: `ch32f103r8t6` or `apm32f103rbt6`.
   * `[language]` Two-letter code for your preferred language (e.g., `en` for English).
5. If the microprocessor is unmarked, try `apm32f103rbt6` firmware first. If a **Drive full** error occurs, disconnect the device, then retry with `ch32f103r8t6`.
6. Copy the selected firmware file to the USB drive. The device should restart automatically with Rad Pro installed.

**Note:**

* Some devices require holding the **Power** button from power-on until the firmware is copied and the device restarts.
* Others may need the **Right/Settings** and **OK/Power** keys pressed together to make the USB drive available.
* You may have to copy the file to the USB drive twice.

**Troubleshooting:**

* If the installation fails, press the **reset switch** next to the USB connector and repeat the process.
* To restore the original firmware, copy the [original firmware](firmware) to the USB drive.
* For persistent issues, follow the [FNIRSI GC-01 alternative installation instructions](install-stlink.md).

## Step 3: Configure the device

Use the following controls to operate your device:

* **Power on/off:** Press and hold the OK/Power key.
* **Switch measurement mode:** Use the Up or Down key.
* **Switch secondary measurement view:** Press the Left/Back key.
* **Reset measurement/dismiss alert:** Press and hold the Left/Back key.
* **Toggle pulse sound (measurement view only):** Press and hold the Right/Settings key.
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

* With 1-byte differential values: [5 pages * (1 timestamp record/page [10 bytes] + 1012 differential records/page [1 byte each])] = 5065 records
* With 2-byte differential values: [5 pages * (1 timestamp record/page [10 bytes] + 506 differential records/page [2 byte each])] = 2535 records

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

