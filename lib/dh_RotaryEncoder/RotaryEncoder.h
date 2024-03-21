#pragma once
#include <Arduino.h>

// Values returned by Gray encoder process (state machine)
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

class RotaryEncoder {

    private:
        uint8_t pinA;
        uint8_t pinB;
        uint8_t pinC;
        bool dir;

        unsigned char state;
        short int ticks = 0;
        bool button_pressed = false;        

    public:
        RotaryEncoder(uint8_t _pinA, uint8_t _pinB, bool _dir);
        RotaryEncoder(uint8_t _pinA, uint8_t _pinB, uint8_t _pinC, bool _dir);
        ~RotaryEncoder();
 
        bool init(void);
        bool feedEvents();
        bool isChanged(void);
        short int getTicks(void);
        bool isPressed(void);

};