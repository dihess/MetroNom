#pragma once
#include <Arduino.h>

// Values returned by Gray encoder process (state machine)
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

class RotaryEncoderMCP {

    private:
        Adafruit_MCP23X17* mcp;
        byte pinA;
        byte pinB;
        byte pinC;
        bool dir;
        byte id;

        unsigned char state;
        short int ticks = 0;
        bool button_pressed = false;        

    public:
        RotaryEncoderMCP(Adafruit_MCP23X17* _mcp, byte _pinA, byte _pinB, bool _dir, byte _id);
        RotaryEncoderMCP(Adafruit_MCP23X17* _mcp, byte _pinA, byte _pinB, byte _pinC, bool _dir, byte _id);
        ~RotaryEncoderMCP();
 
        bool init(void);
        bool feedEvents(byte _pin, unsigned short int _states);
        void feedExternalSwitchEvent(void);
        bool isChanged(void);
        short int getTicks(void);
        bool isPressed(void);

};