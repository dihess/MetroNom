/* config.h */

#pragma once
#include <Arduino.h>

// I2C addresses
#define I2C_SDA 21
#define I2C_SCL 22
#define I2C_SPEED 400000
#define I2C_OLED 0x70
#define I2C_ENCODER 0x20 // not used -> hard coded in Adafruit MCP23017 library?

// OLED ssd1306
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 128x64, 0x3D for 128x32
#define D_MAIN 0 // Display used for main menu
#define D_VALVE_1 1 // Display used for valve 1
#define D_VALVE_2 2 // Display used for valve 2
#define D_VALVE_3 3 // Display used for valve 3
#define D_CAMERA 4 // Display used for SLR camera
#define D_STEPPER 5 // Display used for macro rack stepper

// LED pins
uint8_t ledPin_1 = 33;
uint8_t ledPin_2 = 32;
uint8_t ledPin_3 = 23;
uint8_t ledPin_4 = 22;

// Speaker pin
uint8_t speaker_pin = 2; // ESP32 pin for audio output to speaker

// Roatry encoders with MCP23017
/* the INT pin of the MCP can only be connected to
 * an interrupt capable pin on the ESP32,
*/
const short int arduinoIntPin = 25;
// Assigment of encoder switch pins (directly connected to ESP32)
const short int enc_0_sw_pin = 26; // needs bit 8 set to recognize directly connected button switch
const short int enc_5_sw_pin = 27; // needs bit 8 set to recognize directly connected button switch


// WLAN
const char* const ssid = "AWLAN";
const char* const password = "59ZGH@S44MK1";


