[![Rad Pro Demonstration](docs/img/radpro-video.jpg)](https://www.youtube.com/watch?v=7dpVG1jSLn8)

## Introduction

Rad Pro (formerly FS2011 Pro) is a custom firmware that enhances the capabilities of several low-cost Geiger counters, including the [FS2011](https://www.amazon.com/s?k=fs2011), [FS9000](https://www.amazon.com/s?k=fs9000), [NR-950](https://www.amazon.com/s?k=nr950) and [YT-203B](https://www.amazon.com/s?k=yt-203b). Support for the [Bosean FS-600](https://www.bosean.net/products/nuclear_radiation_detector.html) and [Bosean FS-1000](https://www.bosean.net/products/FS-1000_nuclear_radiation_detector.html) is planned.

<!-- Rad Pro (formerly FS2011 Pro) is a custom firmware that enhances the capabilities of several commercially available Geiger counters, including the [FS2011](https://www.amazon.com/s?k=fs2011), [Bosean FS-600](https://www.bosean.net/products/nuclear_radiation_detector.html) and [Bosean FS-1000](https://www.bosean.net/products/FS-1000_nuclear_radiation_detector.html). Support for other devices, including the [FNIRSI GC-01](https://www.fnirsi.com.cn/product/704376090398953472), [GQ GMC-500](https://www.gqelectronicsllc.com/comersus/store/comersus_viewItem.asp?idProduct=5629) and [GQ GMC-600](https://www.gqelectronicsllc.com/comersus/store/comersus_viewItem.asp?idProduct=5637), is planned. -->

<!-- Rad Pro (formerly FS2011 Pro) is a custom firmware that enhances the capabilities of several commercially available Geiger counters, including the [FS2011](https://www.amazon.com/s?k=fs2011), [Bosean FS-600](https://www.bosean.net/products/nuclear_radiation_detector.html), [Bosean FS-1000](https://www.bosean.net/products/FS-1000_nuclear_radiation_detector.html) and [FNIRSI GC-01](https://www.fnirsi.com.cn/product/704376090398953472). Support for other devices, including the [GQ GMC-500](https://www.gqelectronicsllc.com/comersus/store/comersus_viewItem.asp?idProduct=5629) and [GQ GMC-600](https://www.gqelectronicsllc.com/comersus/store/comersus_viewItem.asp?idProduct=5637), is planned. -->

With Rad Pro, you can:

* Measure instantaneous radiation rate (based on the last 11 pulses or 5-second moving-average).
* Measure average radiation rate (over a time period).
* Measure cumulative dose (stored in non-volatile memory).
* Generate customizable rate and dose alarms.
* Plot historical radiation rate over different time periods (2 minutes, 10 minutes, 1 hour, 6 hours and 24 hours).
* Generate secure passwords (letters & numbers, full ASCII), random hexadecimal and decimal numbers, coin flips, and dice throws (4-sided, 6-sided, 8-sided, 12-sided and 20-sided).
* Log data to the built-in flash memory or live on a computer, optionally sending the data to the https://gmcmap.com or https://radmon.org radiation monitoring websites.

<!-- With Rad Pro, you can:

* Measure instantaneous radiation rate.
* Measure average radiation rate over a time period.
* Measure cumulative dose (stored in non-volatile memory).
* Generate customizable rate and dose alarms.
* Plot historical radiation rate over different time periods (10 minutes, 1 hour, 6 hours, 24 hours and 7 days).
* Log data to built-in flash memory or live on a computer, optionally submitting the data to the [gmcmap.com](https://gmcmap.com), [radmon.org](https://radmon.org) and [safecast.org](https://map.safecast.org) radiation monitoring websites.
* Generate secure passwords (letters & numbers, full ASCII), random hexadecimal and decimal numbers, coin flips, and dice throws (4-sided, 6-sided, 8-sided, 12-sided and 20-sided). -->

## Updates

* Rad Pro 1.3.0 adds configurable Geiger tube conversion factors and data logging support.

<!-- Rad Pro 2.0 adds:

* Adaptive averaging for instantaneous rate measurement.
* Average rate measurement timer.
* Support for new devices: Bosean FS-600, Bosean FS-1000, FS2011 with GD32F103 processor and FNIRSI GC-01.
* Data logging with real-time clock timestamping and data compression.
* Serial port data communication.
* `radpro-tool` for downloading the data log, logging live data, submitting data to radiation monitoring websites and syncing the RTC clock.
* Configurable dead-time compensation at cps level.
* Configurable HV duty cycle for tube voltage control.
* Dead-time measurement.
* [safecast.org](https://map.safecast.org) radiation monitoring website submission.
* Several bugfixes and optimizations.

Note: Rad Pro 2.0 uses a new settings format and requires an installation. -->

## Project overview

* [Legal stuff](docs/legal.md)
<!-- * [Rad Pro Simulator: try before installing](docs/install-simsdl.md) -->
* [Installing on the FS2011](docs/install-fs2011.md)
<!-- * [Installing on the Bosean FS-600 and Bosean FS-1000](docs/install-fs600-fs1000.md) -->
<!-- * [Installing on the FNIRSI GC-01](docs/install-gc01.md) -->
* [Installing on other devices](docs/install-other.md)
* [User's manual](docs/manual.md)
* [radpro-tool](docs/radpro-tool.md)
* [Developers](docs/developers.md)

## Thanks

* Thanks to Alfred90, danielhahn76, felixsteghofer, DG0MG, murkle and cromagn for device testing.
* Thanks to pixitha for the FS2011 (GD32F150 processor) board photo with a J305 Geiger tube, felixsteghofer for the FS2011 (STM32F103 processor) board photo, DG0MG for the Bosean FS-600 board photo and murkle for the Bosean FS-1000 board photo.
* Thanks to the u8g2 team for their excellent display library.
