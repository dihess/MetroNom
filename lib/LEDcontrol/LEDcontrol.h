#pragma once
#include <Arduino.h>

class LEDcontrol {

    private:
        bool active = 0; // 0 = no program
        unsigned short int on_time = 0; // led on [ms]
        unsigned long timestamp;
        unsigned long currentMillis;
        byte pin; // pin number
        /*int melody_1[] = {
         iNOTE_A4
        };

        int noteDurations_1[] = {
         4
        }; */
  

    public:
        LEDcontrol(byte led_pin);
        ~LEDcontrol();
        
        void flash(unsigned short int led_on_time);
        void on();
        void off();
        void update();

};

