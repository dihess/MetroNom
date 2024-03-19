#pragma once
#include <Arduino.h>

class LEDcontrol {

    private:
        bool active = 0; // 0 = no program
        unsigned short int on_time = 0; // led on [ms]
        int pitch; // speaker tone [Hz]

        unsigned long timestamp;
        unsigned long currentMillis;
        uint8_t led_pin; // LED pin number
        uint8_t speaker_pin; // Speaker pin number
        /*int melody_1[] = {
         iNOTE_A4
        };

        int noteDurations_1[] = {
         4
        }; */
  

    public:
        LEDcontrol(uint8_t _led_pin, uint8_t _speaker_pin); 
        ~LEDcontrol();
        
        void flash(unsigned short int led_on_time, int pitch); // beat duration [ms], note pitch [Hz]
        void on();
        void off();
        void update();

};

