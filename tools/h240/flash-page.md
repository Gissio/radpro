# H240 SRAM page loader

The Cortex-M0 loader programs one aligned 4 KiB page at a time over an SRAM
mailbox. The ten permitted addresses run from `0x08000000` through
`0x08009000`. The host programs the vector page last and compares all
128 KiB before and after the transaction. See the
[programming preconditions](../../docs/porting/h240/programming.md).

## Reproduce the bundled loader

Use Clang, LLD and LLVM objcopy 21.1.8. The checked-in loader is 1,584 bytes.

```sh
sh tools/h240/build-flash-page.sh build/h240-flash-page
cmp -- build/h240-flash-page/flash-page.bin tools/h240/flash-page.bin
sha256sum tools/h240/flash-page.bin
```

The accepted SHA-256 is `13857b233cf4718050c90bcba6142b6b48563d3e673628153e5db81be0312e53`.
The linker sets the entry at `0x20000000`, the mailbox at `0x20000630`,
the initial MSP at `0x20000860` and the staging page at `0x20001000`.
The host binds its register and SRAM layout checks to this binary.

Each page uses separate prepare and commit tokens. The loader checks the
sequence, converter state, idle FCI and the old and new page CRCs before
erase. It verifies erased bytes and programmed words, then locks FCI and
reports completion. The host waits for that state and verifies the page
before continuing. A terminal error leaves the loader in SRAM.

[Install and restore commands](flash-install.md)
