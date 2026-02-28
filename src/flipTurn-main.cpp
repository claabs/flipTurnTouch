
/**
 ** *************************************************************************************
 *?    flipTurn-main.cpp
 *       (comments are formatted with VSCode Better Comments extension)
 **  ---------------------------------------------------------------------------------
 **  flipTurn - ESP-Arduino software to send BLE keyboard pagnation commands to
 **    sheet music Apps like Unreal Book
 **  Copyright (C) 2022-2023 Carl W Greenstreet
 *
 **  This program is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 2 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License along
 **  with this program; if not, write to the Free Software Foundation, Inc.,
 **  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **  ---------------------------------------------------------------------------------
 *
 *?   Purpose:  Send BLE pagnation commands to sheet music apps
 *      Project Repository:  https://github.com/claabs/flipTurnTouch
 *
 * *  This variant uses two separate touch pads (forward/back) instead of
 * *  a single foot-switch.  Touching the forward pad sends a page-down
 * *  (keyboard down-arrow) and touching the back pad sends a page-up
 * *  (keyboard up-arrow).  Battery percentage is reported via BLE; long
 * *  presses and colour feedback have been retired in favour of the simpler
 * *  status LED and MAX1704x monitoring.
 *
 *
 *?   Credits (3rd Party Libraries, code snippets, etc)
 *    -------------------------------------------------
 *     This application uses or adapts Open Source code. You can find the source code of their open source projects
 *     along with license information below. We acknowledge and are grateful to these developers for their
 *     contributions to open source.
 *
 **      Project: flipTurn
 **        https://github.com/cwgstreet/flipTurn
 *         Use: Overall software inspiration
 *         Copyright (c) 2022 cwgstreet
 *         License: (GPL-2.0) https://github.com/cwgstreet/flipTurn/blob/master/Licence-software.md
 *
 **      Project: Bounce2  https://github.com/thomasfredericks/Bounce2
 *         Use: Debouncing library for Arduino and Wiring
 *         Copyright (c) 2013 thomasfredericks
 *         License (MIT) https://github.com/thomasfredericks/Bounce2/blob/master/LICENSE
 *
 **      Project: ESP32-BLE-Keyboard  https://github.com/T-vK/ESP32-BLE-Keyboard
 *         Use: BLE Keyboard library for ESP32 devices; used to send pagnation commands to iPad
 *         Copyright (c) 2019 T-vK
 *         License (MIT / GPL3) licence discussion: https://github.com/T-vK/ESP32-BLE-Keyboard/issues/60
 *         Fork used in this project: https://github.com/ShocKwav3/ESP32-BLE-Keyboard
 * 
 **      Project: adafruit/Adafruit MAX1704X  https://github.com/adafruit/Adafruit_MAX1704X
 *         Use: LiPo monitor (MAX1704x series)
 *         Copyright (c) 2022 Limor Fried (Adafruit Industries)
 *         License (BSD) https://github.com/adafruit/Adafruit_MAX1704X/blob/main/license.txt
 *
 ** *************************************************************************************/

// external libraries:
#include <Arduino.h>                        // Arduino framework
#include <BleKeyboard.h>                      // BLE keyboard emulation
#include <Bounce2.h>                          // button debouncing
#include <Wire.h>                             // I2C required for fuel gauge
#include <Adafruit_MAX1704X.h>               // LiPo monitor (MAX1704x series)

// project constants and pin definitions
#include "constants.h"

//? ************** Selective Debug Scaffolding *********************
// Selective debug scaffold: comment out  lines below to disable debugging tests at pre-processor stage
//   Note: #ifdef preprocessor simply tests if the symbol's been defined; therefore #ifdef 0 will not work!
//   Ref: https://stackoverflow.com/questions/16245633/ifdef-debug-versus-if-debug
//? *****************************************************************
// #define DEBUG 1  // uncomment to debug
//? ************ end Selective Debug Scaffolding ********************

// BLE keyboard instance (initial battery level set to 100)
BleKeyboard bleKeyboard(DEVICE_NAME, VENDOR_NAME, 100);

// physical objects
Bounce forwardBtn = Bounce();
Bounce backBtn    = Bounce();
Adafruit_MAX17048 fuelGauge;

// flag recorded after setup runs
bool gaugePresent = false;

// timer used for simple LED blink when not paired
unsigned long ledBlinkTimer = 0;

// helper that stores last reported battery percent so we don't spam BLE
uint8_t lastBatteryPercent = 0;

void setup() {
    Serial.begin(SERIAL_MONITOR_SPEED);
    delay(100);  // give debugger time to open

    // initialise BLE keyboard
    bleKeyboard.begin();

    // configure status LED pin
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);

    // initialise the two touch pads (wired to ground when touched, use pull‑ups)
    pinMode(FORWARD_PIN, INPUT_PULLUP);
    pinMode(BACK_PIN, INPUT_PULLUP);
    forwardBtn.attach(FORWARD_PIN);
    backBtn.attach(BACK_PIN);
    forwardBtn.interval(25);
    backBtn.interval(25);

    // initialise I2C and fuel gauge
    Wire.begin();
    if (!fuelGauge.begin()) {
        Serial.println("ERROR: MAX17048 sensor not found, check wiring");
        // we continue running, but battery readings will stay at 0
    } else {
        gaugePresent = true;
    }

    Serial.println("setup() completed");
}

void loop() {
    // update debounced buttons
    forwardBtn.update();
    backBtn.update();

    // forward pad pressed (falling edge)
    if (forwardBtn.fell()) {
        bleKeyboard.write(KEY_DOWN_ARROW);  // or KEY_RIGHT_ARROW if you prefer
        Serial.println("forward pressed -> page forward");
    }

    // back pad pressed
    if (backBtn.fell()) {
        bleKeyboard.write(KEY_UP_ARROW);  // or KEY_LEFT_ARROW
        Serial.println("back pressed -> page backward");
    }

    // periodically sample battery and push level to host
    if (gaugePresent) {
        float perc = fuelGauge.cellPercent();        // returns a float percentage
        uint8_t soc = (uint8_t)perc;                 // truncate to integer for BLE
        if (soc != lastBatteryPercent) {
            bleKeyboard.setBatteryLevel(soc);
            lastBatteryPercent = soc;
            Serial.print("Battery %=");
            Serial.println(soc);
        }
    }

    // status LED: solid when paired, blink when not
    if (bleKeyboard.isConnected()) {
        digitalWrite(STATUS_LED_PIN, HIGH);
    } else {
        unsigned long t = millis();
        if (((t / 500) % 2) == 0) {
            digitalWrite(STATUS_LED_PIN, HIGH);
        } else {
            digitalWrite(STATUS_LED_PIN, LOW);
        }
    }

    delay(10);  // small sleep to reduce CPU usage
}
