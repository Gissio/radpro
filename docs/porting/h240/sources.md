# H240 source references

| Source | Use |
| --- | --- |
| [CA32F01XX datasheet](https://www.zhwssz.com/uploads/file/20230618/9976584707c5c80c7e8013a9aa091942.pdf) | Cortex-M0 memory map, pin multiplexer, timers and 4 KiB flash erase geometry. |
| [Rad Pro discussion #230](https://github.com/Gissio/radpro/discussions/230) and the [H240 MCU sample](https://github.com/user-attachments/files/30503856/CA32F0156L2-firmware_dump.zip) | Board identification and factory behavior. The sample matches both flash reads of the tested unit. |
| [Meross CA32F0156 firmware sample](https://github.com/openshwprojects/FlashDumps/blob/32c6a8544036bcb7b18566495c42eebf63388406/IoT/zOTAz/Meross_Refoss/589a15dc43797ac319ab7dac5ace43b4.bin) | Independent flash command layout. Decode as Thumb at `0x08002800`; inspect erase at `0x080035c0`, program at `0x08003638` and lock/status at `0x08003628` and `0x08003678`. |
| [Raspberry Pi Debug Probe documentation](https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html) | CMSIS-DAP probe and standard SWD connection. |
