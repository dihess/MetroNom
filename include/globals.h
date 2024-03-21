/* globals.h */

#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>

#include "LEDcontrol.h"
#include "RotaryEncoder.h"

extern RotaryEncoder encoder;
extern uint8_t enc_pinA;
extern uint8_t enc_pinB; 
extern uint8_t enc_pinC; 

// OLED displays
// extern Adafruit_SSD1306 display[];

/* Array of all rotary encoders and their pins */
// extern RotaryEncoderMCP encoders[];
// extern const byte numEncoders;

