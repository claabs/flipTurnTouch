# flipTurnTouch

This repository started life as a foot‑switch driven page‑turner; it has been forked and adapted for a two‑pad, single‑colour design using an Adafruit ESP32‑S3 Feather.

The hardware still emulates a BT‑105‑series Airturn device by sending simple keyboard commands over the BLE Keyboard profile, but the user interface has been changed as follows:

* **Two capacitive touch pads** – one for forward (page down) and one for back (page up).
* **Single‑colour status LED** – flashes when unpaired, stays solid when connected.
* **Battery monitoring via an Adafruit MAX1704x LiPo fuel gauge** – percentage is reported over the BLE battery level characteristic.
* **External reset switch** on the RESET pin is used for power‑down; firmware does not have to manage sleep.

## Operation

Forward pad press sends a page‑forward key (down‑arrow by default); back pad sends a page‑back key (up‑arrow).  No special BLE pairing button is required – the controller advertises as a keyboard and the host handles any bonding.

Battery percentage is continuously updated to the central device; many music apps display this automatically once the keyboard is connected.

## Hardware

* Adafruit ESP32‑S3 Feather with 4 MB Flash / 2 MB PSRAM
* Rechargeable LiPo battery
* Power toggle switch from RESET to GND (manual shutdown)
* Single‑colour LED
* Two capacitive/touch pads wired to D5 and D6
* Copper tape, wires, project box, etc.

## Wiring

* **Forward touch pad** → D5 (GPIO5)
* **Back touch pad** → D6 (GPIO6)
* **Status LED** → D9 (GPIO9) – drive it high for on, use a suitable resistor.
* **LiPo battery** connected to the Feather's battery connector as normal.

## LED Behavior

* Flashing when BLE status is unpaired (500 ms on/off)
* Solid when BLE status is actively paired

## Licence
***Software:*** [GPL v2 Licence](./LICENSE.txt)

[![License: GPL v2](https://img.shields.io/badge/License-GPLv2-blue.svg)](https://www.gnu.org/licenses/gpl-2.0)
