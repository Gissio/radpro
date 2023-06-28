[![Rad Pro Demonstration](docs/img/radpro-video.jpg)](https://www.youtube.com/watch?v=7dpVG1jSLn8)

## Introduction

Rad Pro (formerly FS2011 Pro) is a custom firmware that enhances the capabilities of several low-cost radiation meters/Geiger counters ([FS2011](https://www.amazon.com/s?k=fs2011), [FS9000](https://www.amazon.com/s?k=fs9000), [NR-950](https://www.amazon.com/s?k=nr950) and [YT-203B](https://www.amazon.com/s?k=yt-203b)). Support for the [Bosean FS-600](https://www.bosean.net/products/nuclear_radiation_detector.html) and [Bosean FS-1000](https://www.bosean.net/products/FS-1000_nuclear_radiation_detector.html) is planned.

Rad Pro can be used to:

* Measure instantaneous radiation rate, based on the last 11 pulses or a 5-second moving-average filter.
* Measure average radiation rate over a time period.
* Measure cumulative dose (stored in non-volatile memory).
* Generate customizable rate and dose alarms.
* Plot historical radiation rate over different time periods (2 minutes, 10 minutes, 1 hour, 6 hours and 24 hours).
* Generate secure passwords (letters & numbers, full ASCII), random hexadecimal and decimal numbers, coin flips and dice throws (6-sided, 4-sided, 8-sided, 12-sided and 20-sided).
* Log the measurements on a computer, optionally sending the data to the https://gmcmap.com or https://radmon.org radiation monitoring websites.

## Updates

* Rad Pro 1.2.2 adds support for devices with a GD32 microcontroller and data submission to the https://gmcmap.com radiation monitoring website.

## Project overview

* [Before installing](docs/legal.md)
* [Installing on the FS2011/FS9000/NR-950/YT-203B](docs/install-fs2011.md)
* [Installing on other devices](docs/install-other.md)
* [User's manual](docs/manual.md)
* [Developers](docs/developers.md)

## Thanks

* Special thanks to the u8g2 team for their excellent LCD library.
* Special thanks to Alfred90 and danielhahn76 for device testing.
