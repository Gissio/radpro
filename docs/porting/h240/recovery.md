# H240 factory recovery

Keep two matching 128 KiB captures of `0x08000000–0x0801ffff` from the
individual unit and record their SHA-256. The procedure restores the ten
application pages from this backup and verifies all 128 KiB byte for byte.
The external PUYA SPI NOR and controller option areas are outside its write
range.

For a complete Rad Pro image, place the device in standby and run the
host's `restore` action with the exact installed image and its hash.
After an interrupted write, recovery requires the same powered SRAM
loader session, an idle error-free controller, and a fresh complete flash
capture. Reboot once the factory readback matches the backup.

Supervised restores reproduced the tested unit's factory flash and
restarted its charge runtime. The automatic switch to the programming
clock was checked in OpenOCD mocks. Recovery after power loss during a
page write remains unqualified.

[Restore commands](../../../tools/h240/flash-install.md) · [Programming preconditions](programming.md)
