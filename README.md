# flipTurnTouch

flipTurn is a wireless, battery-operated / rechargeable touch operated page-turner designed to work with digital sheet music apps like MobileSheets or forScore.  Based on an ESP32-S3 microcontroller, the device uses BLE (Bluetooth Low Energy) to communicate with the app.

My purpose for this device is to mount it to a cymbal stand on my drum kit so I can turn pages easily while playing. Capacitive touch operation is preferred so I don't need to engineer hardware strong enough to take a hit or physical press while suspended on a long lever arm.

## Operation

The flipTurn page turner emulates a BT-105 series Airturn device and sends a limited set of simple keyboard commands. 

Using bluetooth, flipTurn pairs the page turner peripheral with central device (e.g. a phone or tablet).

Once paired, the peripheral will automatically re-pair upon startup.

## Hardware

* [Adafruit ESP32-S3 Feather with 4MB Flash 2MB PSRAM](https://www.adafruit.com/product/5477)
* Rechargeable LiPo battery
* Power toggle switch
* Single color LED
* Copper tape for capacitive touch operation
* Various wire
* Generic plastic project box that fits it all

## Licence
***Software:*** [GPL v2 Licence](./Licence-software.md)

[![License: GPL v2](https://img.shields.io/badge/License-GPLv2-blue.svg)](https://www.gnu.org/licenses/gpl-2.0)
