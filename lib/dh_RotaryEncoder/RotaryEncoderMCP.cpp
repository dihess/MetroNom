/* Class to handle rotary encoders attached to MCP23017 port multiplexer
 * 
 * Copyright 2023 Dirk Hessing, licensed under GNU GPL v3.
 * Contact dirk.hessing@googlemail.com
 * 
 * Gray decoder mechanism derived from Ben Buxton:
 * https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/Rotary.cpp
 *   
*/

#include "RotaryEncoderMCP.h"

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
RotaryEncoderMCP::RotaryEncoderMCP(Adafruit_MCP23X17* _mcp, byte _pinA, byte _pinB, bool _dir, byte _id) {
    mcp = _mcp;
    pinA = _pinA;
    pinB = _pinB;
    pinC = -1;
    dir = _dir;
    id = _id;
    // Initialize state
    state = R_START;
}

/* Constructor for encoders with button switch*/
RotaryEncoderMCP::RotaryEncoderMCP(Adafruit_MCP23X17* _mcp, byte _pinA, byte _pinB, byte _pinC, bool _dir, byte _id) {
    mcp = _mcp;
    pinA = _pinA;
    pinB = _pinB;
    pinC = _pinC;
    dir = _dir;
    id = _id;
    // Initialize state
    state = R_START;
}

RotaryEncoderMCP::~RotaryEncoderMCP() {}

/* Initialize object in the MCP */
bool RotaryEncoderMCP::init(void) {
    // initialize encoder A and B and C (button switch) pins
    if ( (mcp == nullptr) || (pinA > 15) || (pinB > 15) ) {
        #ifdef ERROR
        Serial.println("[RotaryEncoderMCP] Initialization failed: invalid parameters");
        #endif
        return 0;
    }
    mcp->pinMode(pinA, INPUT_PULLUP);
    mcp->setupInterruptPin(pinA, CHANGE);
    mcp->pinMode(pinB, INPUT_PULLUP);
    mcp->setupInterruptPin(pinB, CHANGE);

    if ( (pinC != -1) && (pinC < 16) ) { // switch button pin connected to MCP
        mcp->pinMode(pinC, INPUT_PULLUP);
        mcp->setupInterruptPin(pinC, CHANGE);
        #ifdef DEBUG
        Serial.print("[RotaryEncoderMCP::init] Encoder ");
        Serial.print(id);
        Serial.print(" button switch attached on pin: ");
        Serial.println(pinC);
        #endif
    } 
        
    return true;
}

bool RotaryEncoderMCP::feedEvents(byte _pin, unsigned short int _states) {
    byte pin = _pin;
    unsigned short int states = ~_states; // inversed 16-bit port states
    unsigned short int pinstate;

    if ( (pin != pinA) && (pin != pinB) && (pin != pinC) ) { // interrupt not triggered by this encoder instance
        return 0; 
    } else if ((pin == pinA) || (pin == pinB)) { // rotary event triggered by this encoder instance   
        
        pinstate = 0; // 2-bit value of pinA and pinB
        if (pinA < pinB) {
            pinstate = ((states & (1 << pinB)) >> (pinB - 1)) | ((states & (1 << pinA)) >> pinA); 
        } else {
            pinstate = ((states & (1 << pinB)) >> pinB ) | ((states & (1 << pinA)) >> (pinA - 1)); 
        }  
        state = ttable[state & 0xf][pinstate];
        #ifdef DEBUG
        Serial.print("[RotaryEncoderMCP::feedEvents] Received interrupt on pin: ");
        Serial.print(pin);
        Serial.print(" states=0b");
        Serial.print(states, 2);
        Serial.print(" pinstate=0b");
        Serial.print(pinstate, 2);
        Serial.print(" state=0x");
        Serial.println(state, 16);
        #endif
        if ( (state & 0x30) == DIR_CW ) {
            if (dir) { // switch bits if inverse rotation is set
                ticks--;
            } else {
                ticks++;
            }
            #ifdef DEBUG
            Serial.print("[RotaryEncoderMCP::feedEvents] Clockwise event received for id: ");
            Serial.println(id);
            Serial.print("[RotaryEncoderMCP::feedEvents] #ticks: ");
            Serial.println(ticks);
            #endif
        } else if ( (state & 0x30) == DIR_CCW ) {
            if (dir) { // switch bits if inverse rotation is set
                ticks++;
            } else {
                ticks--;
            }
            #ifdef DEBUG
            Serial.print("[RotaryEncoderMCP::feedEvents] CounterClockwise event received for id: ");
            Serial.print(id);
            Serial.print(" tickscounter=");
            Serial.println(ticks);
            Serial.println();
            #endif
        }
        // mcp->readGPIOAB(); // clear MCP interrupt
        return 1;
    } else if (pin == pinC) { // button push triggered by this encoder instance   
        if ( states & (1 << pinC) )
            button_pressed = true; 
        // mcp->readGPIOAB(); // clear MCP interrupt
        return 1;
    } else {
        #ifdef ERROR
        Serial.println("[RotaryEncoderMCP::feedEvents] Unknown error during decode");
        #endif
        return 0;
    } 
}

void RotaryEncoderMCP::feedExternalSwitchEvent(void) {
    button_pressed = true;
}

bool RotaryEncoderMCP::isChanged(void) {
    if ( (ticks != 0) || button_pressed ) {
        return 1;
    } else {
        return 0;
    }
}

short int RotaryEncoderMCP::getTicks(void) {
    short int current_ticks = ticks;
    ticks = 0;
    return current_ticks;
}

bool RotaryEncoderMCP::isPressed(void) {
    if (button_pressed) {
        button_pressed = false;
        return 1;
    } else {
        return 0;
    }
}