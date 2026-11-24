# GC-01 H240-VER2.2 hardware

The tested board is marked **H240-VER2.2** and uses a **CA32F0156L2**.
The GC-01 has its own power supply. The tested probe is a YD-RP2040
2002-v1.3 running Debugprobe on Pico, connected at 100 kHz SWD.

With both boards unpowered, connect the signal and ground wires. J1
positions run toward the GC-01 USB connector, with ground nearest it.

| GC-01 | Signal | YD-RP2040 |
| --- | --- | --- |
| J1-1 | Target logic rail | Leave open |
| J1-2 | SWDIO (PB2) | GP3 |
| J1-3 | SWCLK (PB3) | GP2 |
| J1-4 | Ground | GND |

| Function | Pins | Evidence |
| --- | --- | --- |
| Up, Down, Left, Right, Center | PB11, PB13, PB14, PB12, PB15 | Active-low GPIO transitions on the tested board. |
| Panel | SPI1 on PC6/PC8. CS PC9, reset PC10, D/C PC11. | Factory pin configuration and a displayed RGB pattern. |
| Backlight | PC12, TIM1 channel 2 | Live brightness and timeout behavior. |
| Detector | PA10, active low, TIM3 | Factory input configuration and observed raw counts on Rad Pro. |
| Converter drive | PA15 AF6, TIM14 channel 3 | Factory GPIO and timer readback. |

Factory USB charge mode and Rad Pro standby are the starting states for
SWD programming. Verify PA15 is low and TIM14 is gated before halting
the core. The converter circuit carries high voltage. Use identified
low-voltage signals for probing.

[Debugprobe firmware and wiring](https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html) · [Board evidence](sources.md)
