# Installing Rad Pro on FNIRSI GC-01/JOY-IT JT-RAD01 Geiger counters

This guide explains how to install the Rad Pro firmware on FNIRSI GC-01 and JOY-IT JT-RAD01 Geiger counters.

## Required supplies

To install Rad Pro, you'll need:

* USB data cable (ensure it supports data transfer, not just charging)
* Philips screwdriver

## Step 1: Inspect the device

![FNIRSI GC-01 circuit board types](img/gc-01-board-type.jpg)

1. Power off the Geiger counter.
2. Unscrew the back case and carefully open the device.
3. Note the microprocessor type (square chip), which should be CH32F103R8T6 (WCH) or APM32F103RBT6 (Geehy). The picture above should help you identify the MCU. **WARNING:** Rad Pro is not compatible with devices using a CACHIP microprocessor.
4. Note the Geiger-Müller tube type (marked on the glass as J614, J321, J305 or M4011). For unmarked tubes measuring 55 or 65 mm, assume J614 or J613, respectively.
5. Close and reassemble the device.

## Step 2: Flash the firmware

1. Download and extract the latest `radpro-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
2. Navigate to the `fnirsi` folder, then to the `install` subfolder. Select the appropriate firmware file: `radpro-fnirsi-gc01_[mcu]-[language]-x.y.z-install.bin`. `[mcu]` is your device's microprocessor (`ch32f103r8t6` or `apm32f103rbt6`), and `[language]` is the two-letter code for your preferred language (e.g., `en` for English).
3. Connect the device to a USB 2.0 port on a Windows computer using the USB data cable. **WARNING:** Installation may fail on macOS or Linux, or USB 3.0 ports.
4. Power on the device. A USB drive should become available.
5. Copy the firmware file onto the USB drive. The device should restart automatically with Rad Pro installed.

**Note:** Some devices require holding the power button from power-on until the firmware is copied and the device restarts. Others may need the Right/Settings key pressed simultaneously with the power key to make the USB drive available.

**Troubleshooting:** If the installation fails, press the reset switch next to the USB connector and repeat the process. To restore the device, use the [original firmware](firmware). For persistent issues, refer to the [FNIRSI GC-01 alternative installation instructions](install-stlink.md).

## Step 3: Configure the device

Use the following controls to operate your device:

  * **Power on/off:** Press and hold the OK/Power key.
  * **Switch measurement mode:** Use the Up or Down key.
  * **Switch secondary measurement view:** Press the Left/Back key.
  * **Reset measurement/dismiss alarm:** Press and hold the Left/Back key.
  * **Toggle pulse sound (measurement view only):** Press and hold the Down key.
  * **Sleep display (measurement view only):** Press the Ok/Power key.
  * **Access settings:** Press the Right/Settings key.
  * **Navigate options:** Use the Up or Down key.
  * **Select option:** Press the OK/Power or Right/Settings key.
  * **Go back:** Prses the Left/Back key.
  * **Toggle lock mode:** Press and hold both the OK/Power and Left/Back keys.

To configure the device:

1. Go to **Settings > Geiger tube > Sensitivity** and select the option matching your Geiger-Müller tube.
2. In **Geiger tube > HV Profile**, choose an appropriate profile:
  * **Energy-saving:** Lowest power consumption, ideal for background radiation level. If no tube counts are detected, configure a custom HV profile to optimize power usage.
  * **Accuracy:** Higher power consumption, suitable for elevated radiation levels.
  * **Factory default:** Highest power consumption, uses the original firmware profile.
3. If the device registers spontaneous key presses, select the **Energy-saving** HV profile or configure a custom HV profile to resolve the issue.

## Step 4: Final steps

If you find Rad Pro useful, consider:

* Watching the [Rad Pro GitHub repository](https://github.com/Gissio/radpro) for release notifications.
* Starring the project to show your support.

## Hardware-specific notes

### USB data connection

Use a USB data cable to connect the GC-01 to a computer.

### FNIRSI GC-01/JOY-IT JT-RAD01 with CH32F103R8 (WCH) microprocessor

* Enable **Data Mode** in the settings to establish a USB data connection. Disable it when not in use to save power.

<!-- Calculated as follows:

* With 1-byte differential values: [6 pages * (1 timestamp entry/page [10 bytes] + 1012 differential entries/page [1 byte each])] = 6078 entries
* With 2-byte differential values: [6 pages * (1 timestamp entry/page [10 bytes] + 506 differential entries/page [2 byte each])] = 3042 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 6,078 data points. At 20 cpm (normal radiation levels), this supports:
  * 126 days at 60-minute intervals
  * 21 days at 10-minute intervals
  * 4 days at 1-minute intervals
  * 16 hours at 10-second intervals
  * ~2 hours at 1-second intervals

* **HV profile settings:**
  * Factory default: 9.207 kHz frequency, 75% duty cycle.
  * Energy-saving: 5 kHz frequency, 1.5% duty cycle.

### FNIRSI GC-01/JOY-IT JT-RAD01 with APM32F103R8 (Geehy) processor

* The USB data connection is always available, no need to enable **Data Mode**.

<!-- Calculated as follows:

* With 1-byte differential values: [51 pages * (1 timestamp entry/page [10 bytes] + 1012 differential entries/page [1 byte each])] = 51663 entries
* With 2-byte differential values: [51 pages * (1 timestamp entry/page [10 bytes] + 506 differential entries/page [2 byte each])] = 25857 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 61,793 data points. At 20 cpm (normal radiation levels), this supports:
  * 1077 days at 60-minute intervals
  * 179 days at 10-minute intervals
  * 35 days at 1-minute intervals
  * 6 days at 10-second intervals
  * 14 hours at 1-second intervals

* **HV profile settings:**
  * Factory default: 47.058 kHz frequency, 50% duty cycle.
  * Energy-saving: 5 kHz frequency, 1.5% duty cycle.
