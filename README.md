[![Rad Pro Simulator - Try me!](https://gissio.github.io/radpro-simulator/radpro-title-tryme.png)](https://gissio.github.io/radpro-simulator/)

[![License](https://img.shields.io/github/license/Gissio/radpro)](https://github.com/Gissio/radpro/blob/main/LICENSE)
[![Last Commit](https://img.shields.io/github/last-commit/Gissio/radpro)](https://github.com/Gissio/radpro/commits/main/)
[![Build Status](https://github.com/Gissio/radpro/actions/workflows/build-all.yml/badge.svg)](https://github.com/Gissio/radpro/actions/workflows/build-all.yml)
[![GitHub release](https://img.shields.io/github/v/release/Gissio/radpro)](https://github.com/Gissio/radpro/releases)

Try the [Rad Pro simulator](https://gissio.github.io/radpro-simulator/)!

## Introduction

**Rad Pro** is a custom firmware designed to enhance the capabilities of supported Geiger counters. It provides advanced measurement tools, data logging, alerting, and visualization features beyond stock firmware.

With Rad Pro, you can:

* Monitor radiation levels in real time
* Compute statistically meaningful averages
* Track accumulated radiation dose
* Configure alerts for rate and dose thresholds
* Visualize historical trends over multiple time scales
* Log and export measurement data
* Stream data to external platforms ([gmcmap.com](https://gmcmap.com), [radmon.org](https://radmon.org), [safecast.org](https://map.safecast.org), [opensensemap.org](https://opensensemap.org))
* Generate high-quality random data

## Supported Devices

Before installing, read the [legal disclaimer](docs/legal.md).

Supported devices:

* [FS2011/YT-203B installation guide](docs/devices/FS2011/install.md)
* [Bosean FS-600/Bosean FS-1000 installation guide](docs/devices/Bosean%20FS-600,%20FS-1000/install.md)
* [Bosean FS-5000 installation guide](docs/devices/Bosean%20FS-5000/install.md)
* [FNIRSI GC-01/JOY-IT JT-RAD01 installation guide](docs/devices/FNIRSI%20GC-01/install.md)
* [FNIRSI GC-03 installation guide](docs/devices/FNIRSI%20GC-03/install.md)
* [GQ GMC-800 installation guide](docs/devices/GQ%20GMC-800/install.md)

## Documentation

* [Rad Pro user's manual](docs/users-manual.md) – Easy guide to using Rad Pro
* [Rad Pro reference manual](docs/reference-manual.md) – Technical reference for Rad Pro
* [Ionizing radiation field guide](https://github.com/Gissio/ionizing-radiation-field-guide) – Learn about ionizing radiation
* [Rad Pro developers's manual](docs/developers.md) – Technical details for developers

## Tools & Software

* [GeigerLog](https://sourceforge.net/projects/geigerlog/) – Data analysis software for PC
* [Rad Pro Log Viewer](https://github.com/mayrthom/RadPro-LogViewer) – Data analysis app for Android

## Acknowledgements

* Rad Pro's user interface leverages the industry-standard [OpenBridge 6.1](https://www.openbridge.no/) design system.
* Special thanks to `Alfred90`, `danielhahn76`, `felixsteghofer`, `DG0MG`, `murkle`, `cromagn`, `ivohar`,`mryndzionek`, `JantarX`,  `mizsha`, `Tig3rch3n`, `Arparela`, `w01fdev`, `martin-machacek`, `dsl71`, `Davegsm82`, `alfmck`, `jonwhite227`, `dc1rdb`, `ihrapsa`, `Montecri`, `boromix`, `maxobe`, `TheAsgard`, `ugopapetti`, `NuclearPhoenixx`, `technils`, `Ilja-code-art`, `ICantMakeThings`, `mayrthom`, `hackra76`, `plamenbe`, `bwisn`, `CalmWookie`, `guestus`, `fonix232`, `iROOT`, `starchaser01`, `ErrorTPkR`, `1n5aN1aC`, `jdogg836`, `ge0rg` and many others for their invaluable testing and feedback.
* Special thanks to `pixitha` for FS2011 (GD32F150) board photos with a J305 Geiger tube, `felixsteghofer` for FS2011 (GD32F103) board photos, `DG0MG` for Bosean FS-600 board photos, `murkle` for Bosean FS-1000 board photos, `ivohar` for FNIRSCI GC-01 schematics, `oOblik` for Bosean FS-5000 board photos, and `guener` for FNIRSI GC-03 board photos.
