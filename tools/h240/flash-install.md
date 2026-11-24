# H240 SWD installation and recovery

This experimental package supports the **GC-01 H240-VER2.2** with a
**CA32F0156L2** and the factory image identified below. Open a terminal
in the extracted `h240-ca32f0156l2` directory. Use OpenOCD 0.12,
CMSIS-DAP, Python 3 and a POSIX host with `cmp` and `sha256sum`.

With both boards unpowered, connect J1-2 (SWDIO), J1-3 (SWCLK) and J1-4
(ground). On the tested Debugprobe-on-Pico probe these go to GP3, GP2
and GND respectively. J1-4 is nearest the GC-01 USB connector. Leave
J1-1 open and power the GC-01 separately. The converter carries high
voltage. Use the identified low-voltage SWD signals.

## Back up the factory firmware

Connect the GC-01 to USB and turn it off with the center button to enter
factory charge mode. Keep the device and probe powered throughout the
capture, programming and verification.

```sh
backup_dir=$(mktemp -d ./h240-factory.XXXXXX) || exit
for capture in factory-1.bin factory-2.bin; do
  openocd -f swd/openocd-sram.cfg \
    -c 'h240.cpu configure -defer-examine' \
    -c 'target create h240.mem mem_ap -dap h240.dap -ap-num 0' \
    -c 'gdb_port disabled' -c init -c 'poll off' \
    -c 'targets h240.mem' \
    -c "dump_image $backup_dir/$capture 0x08000000 0x20000" \
    -c shutdown || exit
done
cmp -- "$backup_dir/factory-1.bin" "$backup_dir/factory-2.bin" || exit
factory="$backup_dir/factory-1.bin"
sha256sum "$factory"
```

Keep copies of both matching 128 KiB captures. The accepted factory
SHA-256 is `604638ea4dbdd9553c7803e87cf8ce7bdbf1ad0a3afc52028293f7fca92ed46f`.
A different factory image requires separate qualification.

## Install or restore

In the same terminal, select and verify the packaged firmware.

```sh
(cd firmware && sha256sum -c SHA256SUMS) || exit
read -r image_sha image_file < firmware/SHA256SUMS || exit
image="firmware/$image_file"
python3 swd/run-flash-page.py install "$factory" "$image" "$image_sha"
```

The runner verifies the packaged loader, the live preimage and the
converter-off state before writing. It programs only the first ten
4 KiB pages with the vector page last, then compares all 128 KiB.
After a successful full comparison, disconnect USB and battery power,
then reconnect and verify the measurement screen. The core stays in the
SRAM loader until this power cycle.

For factory recovery from a complete installation, hold the center button
to put Rad Pro in standby. Use the saved backup and the exact installed image
with its hash.

```sh
python3 swd/run-flash-page.py restore "$factory" "$image" "$image_sha"
```

After a successful full comparison, power-cycle the GC-01 and verify the
factory screen.
If a write stops, leave the device powered and inspect the controller and
loader before halt, reset or retry. The transaction log and captures are
in the directory printed by the runner. Partial-write recovery requires the
same verified SRAM loader, an idle error-free controller and a fresh
128 KiB capture. Recovery after power loss during a write is unqualified.
