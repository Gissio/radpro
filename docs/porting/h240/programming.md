# H240 flash programming

The tested programmer uses CMSIS-DAP and OpenOCD's Cortex-M MEM-AP to run
an SRAM page loader. Writes are limited to `0x08000000–0x08009fff`.
The loader erases and verifies aligned 4 KiB pages, then programs aligned
32-bit words. Flash controller registers are based at `0x40020400`.
The [CA32F01XX datasheet](https://www.zhwssz.com/uploads/file/20230618/9976584707c5c80c7e8013a9aa091942.pdf#page=39)
specifies the page geometry. The [flash register sources](sources.md)
identify the firmware used to confirm the command layout.

Programming uses the factory 64 MHz PLL configuration, analog timing
register `0x40021450 = 6` and FCI CR wait-state bit 8. The host requires
idle FCI and the converter off, with PA15 low and TIM14 gated. During
installation it verifies the 128 KiB preimage, programs and reads back
each page, then compares all 128 KiB. The vector page is written last.
The factory bytes at `0x0800a000–0x0801ffff` are preserved.

A successful run leaves the core in the SRAM loader. Reboot after the
final comparison and verify the expected firmware starts. Following an
interruption, leave the board powered and inspect the controller and
loader before halting, resetting or retrying.

[Host procedure](../../../tools/h240/flash-install.md) · [Recovery](recovery.md)
