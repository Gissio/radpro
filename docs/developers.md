# Developers

To build the software, follow these steps:

* Install [Visual Studio Code](https://code.visualstudio.com/).
* From Visual Studio Code, install the [platform.io](https://platformio.org/) extension.
* Open the `platform.io` folder to begin building the firmware.
* Once you've built the firmware, sign the resulting binary using Python and the `tools/sign-firmware.py` script.
* You can also build the software as a simulator by opening the project's root folder from Visual Studio Code. You'll need the `sdl2` library, which you can install using the [vcpkg](https://vcpkg.io/en/getting-started.html) package manager.
