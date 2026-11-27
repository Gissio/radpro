# Screenshot generator

Generates the screenshots in [docs/img](../img) (`view-*.png`, `menu-*.png`) and the title images (`radpro-title-tryme.png`, `radpro-media.png`) from the firmware, so they can be updated after UI changes.

```sh
python generate.py                     # all screenshots, into docs/img
python generate.py view-history        # some of them
python generate.py --out preview       # into another folder
python generate.py --list
```

`generate.py` builds the simulator firmware (`platform.io/src`, with the flags of the `radpro-color-landscape` target in `CMakeLists.txt`) into a headless host program. Then it runs one short script per screenshot and saves the image at 2×, like the simulator window. [harness.c](harness.c) replaces `main.c` and the simulator files that need SDL or the host clock. Screenshots are deterministic: the simulated pulses use a fixed seed, and the clock runs with the simulation.

Each screenshot is a script in `SCENARIOS` in `generate.py`, starting from a freshly powered-on counter with default settings. The script commands are listed in `harness.c`; `clock HH:MM` sets the time shown in the title bar. Optionally, a rectangle can be highlighted: the rest of the image is dimmed.

The title images are `COMPOSITES`: one script takes several named screenshots (`screenshot NAME`), which are laid out as 640×480 tiles. The title image also uses [monochrome-photo.png](monochrome-photo.png), a photo of a counter with a monochrome display, and gets the "Try me!" ribbon (Arial Bold). The title image used by the README is served by the simulator website, so copy it there after regenerating it.

Needs gcc on the PATH (on Windows, MSYS2 UCRT64; run it from PowerShell or cmd) and Pillow.
