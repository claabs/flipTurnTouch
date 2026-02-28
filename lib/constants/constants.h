/* ***************************************************************
 * myConstants.h - library listing all user-defined constants
 *
 *  C W Greenstreet, Ver1, 27Sep21
 *    MIT Licence - Released into the public domain
 *
 ** ************************************************************* */

#ifndef MY_CONSTANTS_H  // header guard
#define MY_CONSTANTS_H

#include <Arduino.h>

//******************************************************
//? Constants
//*******************************************************/

constexpr int FORWARD_PIN      = T5;  // forward touch pad input (active low)
constexpr int BACK_PIN         = T6;  // back touch pad input (active low)
constexpr int STATUS_LED_PIN   = GPIO_NUM_9;  // single‑colour status LED (active high)

constexpr int SERIAL_MONITOR_SPEED = 115200;

constexpr char * DEVICE_NAME = "flipTurnTouch";  // BLE device name
constexpr char * VENDOR_NAME = "claabs";  // BLE vendor name


// *******************************************************
//   Other constants
// *******************************************************

#endif  // end header guard