/* Class to handle rotary encoders attached to ESP32
 * 
 * Copyright 2024 Dirk Hessing, licensed under GNU GPL v3.
 * Contact dirk.hessing@googlemail.com
 * 
 * Gray decoder mechanism derived from Ben Buxton:
 * https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/Rotary.cpp
 *   
*/

#include "RotaryEncoder.h"

#define ERROR
// #define DEBUG

#define R_START 0x0
// #define GRAY_HALF_STEP


// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

#ifdef GRAY_HALF_STEP

// Use the half-step state table (emits a code at 00 and 11)
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};

#else

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

#endif

/* Constructor for encoders without button switch*/
RotaryEncoder::RotaryEncoder(uint8_t _pinA, uint8_t _pinB, bool _dir) {
    pinA = _pinA;
    pinB = _pinB;
    pinC = -1;
    dir = _dir;
    // Initialize state
    state = R_START;
}

/* Constructor for encoders with button switch*/
RotaryEncoder::RotaryEncoder(uint8_t _pinA, uint8_t _pinB, uint8_t _pinC, bool _dir) {
    pinA = _pinA;
    pinB = _pinB;
    pinC = _pinC;
    dir = _dir;
    // Initialize state
    state = R_START;
}

RotaryEncoder::~RotaryEncoder() {}

/* Initialize object */
bool RotaryEncoder::init(void) {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    #ifdef DEBUG
    pinMode(12, OUTPUT);
    digitalWrite(12,false);
    #endif

    if ( pinC != -1 ) { // switch button pin connected
        pinMode(pinC, INPUT_PULLUP);
        #ifdef DEBUG
        Serial.print("[RotaryEncoder::init] Encoder ");
        Serial.print(" button switch attached on pin: ");
        Serial.println(pinC);
        #endif
    } 
        
    return true;
}

bool RotaryEncoder::feedEvents() {
    uint8_t valueA, valueB, valueC;
    static unsigned long int last_millis;
    unsigned char pinstate;

    #ifdef DEBUG
    static int counter = 0;
    #endif

    valueA = digitalRead(pinA); // read CLK
    valueB = digitalRead(pinB); // read DT

    if (pinC > -1) {
        valueC = !digitalRead(pinC); 
    } else {
        valueC = -1;
    }

    if ( !valueC ) { // rotary tick event triggered interrupt  
        // Grab state of input pins.
        pinstate = (valueB << 1) | valueA;
        // Determine new state from the pins and state table.
        state = ttable[state & 0xf][pinstate];
        // Return emit bits, ie the generated event.
        // return state & 0x30;

        #ifdef DEBUG
        Serial.printf("[RotaryEncoder::feedEvents] Counter:%i - valueA:%i valueB:%i valueC:%i - state:%i pinstate:%i\n",counter, valueA, valueB, valueC, state, pinstate);
        counter++;
        #endif

        if ( (state ) == DIR_CW ) {
            if (dir) { // switch bits if inverse rotation is set
                ticks--;
            } else {
                ticks++;
            }
            #ifdef DEBUG
            Serial.print("[RotaryEncoder::feedEvents] Clockwise event received, ticks now: ");
            Serial.println(ticks);
            #endif
        } else if ( (state ) == DIR_CCW ) {
            if (dir) { // switch bits if inverse rotation is set
                ticks++;
            } else {
                ticks--;
            }
            #ifdef DEBUG
            Serial.print("[RotaryEncoder::feedEvents] CounterClockwise event received, ticks now: ");
            Serial.println(ticks);
            #endif
        }
        return 1;
    } else if (valueC) { // button push triggered interrupt
        button_pressed = true; 
        #ifdef ERROR
        Serial.println("[RotaryEncoder::feedEvents] Button pressed");
        #endif
        return 1;
    } else {
        #ifdef ERROR
        Serial.println("[RotaryEncoder::feedEvents] Unknown error during decode");
        #endif
        return 0;
    } 
}

bool RotaryEncoder::isChanged(void) {
    if ( (ticks != 0) || button_pressed ) {
        return 1;
    } else {
        return 0;
    }
}

short int RotaryEncoder::getTicks(void) {
    short int current_ticks = ticks;
    ticks = 0;
    return current_ticks;
}

bool RotaryEncoder::isPressed(void) {
    if (button_pressed) {
        button_pressed = false;
        return 1;
    } else {
        return 0;
    }
}