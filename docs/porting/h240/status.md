# GC-01 H240-VER2.2 support

This experimental French target runs Rad Pro on the H240-VER2.2 board
with a CA32F0156L2. It provides a color interface, five-button navigation,
standby and raw pulse measurements in CPS, CPM and cumulative counts.
Settings live in RAM. Dose conversion, dose alarms, logging and host
communications are outside this profile.

## Build

```sh
pio run -d platform.io -e h240-ca32f0156l2-fr
```

PlatformIO produces `platform.io/.pio/build/h240-ca32f0156l2-fr/radpro-h240.bin`
with the application CRC filled in. It also emits the raw BIN, ELF and
linker map. The image uses the first ten 4 KiB pages of internal flash.
Run `tools/sign.py` from a fresh release directory to assemble the H240
binary, checksum and SWD installation kit. Installation requires the
qualified factory profile and a full backup from the device.

## Architecture

| Area | Implementation |
| --- | --- |
| Startup | CA32 Cortex-M0 at 16 MHz using HSI with a 1 kHz system tick. The linker reserves 40 KiB at `0x08000000` and 1,536 bytes of stack in the 8 KiB SRAM budget. |
| Board I/O | [`platform.io/src/ca32`](../../../platform.io/src/ca32) owns CA32 GPIO, display, buttons, power, ADC and Geiger timing. |
| Display | The GC9307-family color panel uses SPI1 and the existing MCU renderer. TIM1 controls its backlight. |
| Pulse acquisition | TIM3 samples the active-low PA10 detector input at about 15 kHz. TIM14 drives the converter on PA15 at a nominal 2 kHz and 20% duty. |
| Measurements | The shared `RADPRO_RAW_COUNTER` profile consumes hardware counter deltas and displays rates, elapsed time and totals. |
| Image integrity | The H240 build fills the linker-reserved CRC word. Startup checks the CRC and a healthy display before starting measurements. |

## Validation

The hardware bring-up build passed cold boot, view and menu navigation,
and standby and wake on one H240-VER2.2. It reported 81 background pulses in 7 minutes 21 seconds
(11.0 CPM). A full 128 KiB readback matched the expected image. A
supervised factory restore reproduced the original 128 KiB and booted.
The current build increases the text buffer for multi-digit rendering
and keeps measurements disabled after a latched display fault. Host tests
reproduce both failures and verify the corrections. The current build
still needs a hardware retest. Converter waveform, detector response and
recovery after power loss during a flash write require separate tests.

The H240 and reference `fs2011-stm32f051c8-en` and
`fnirsi-gc01_ch32f103r8-en` targets built with PlatformIO Core 6.2.0 and
ST STM32 20.0.0. `sh tests/test-raw-counter.sh` covers counter wrap,
delayed processing, isolated pulses, CPS/CPM formatting, the numeric
renderer and the display-fault guards.

[Hardware wiring](hardware.md) · [Register provenance](reverse-engineering.md) · [Source references](sources.md)
