#include "LEDControl.h"
#define BUZZZER_PIN  2
LEDcontrol::LEDcontrol(byte led_pin) {
    active = 0; // 0 = no program
    on_time = 0; // led on [ms]
    timestamp = millis();
    pin = led_pin; // pin number

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH); // HIGH => LED off
}

LEDcontrol::~LEDcontrol() {}

void LEDcontrol::flash(unsigned short int led_on_time) {
    on_time = led_on_time;
    timestamp = millis();
    digitalWrite(pin, LOW); // LOW => LED on
   tone(BUZZZER_PIN, 440);
        

    active = 1;
}


void LEDcontrol::on() {
    digitalWrite(pin, LOW); // LOW => LED on
    return;
}

void LEDcontrol::off() {
    digitalWrite(pin, HIGH); // HIGH => LED off
    return;
}

void LEDcontrol::update() {
     /*
    Call this function in loop() to flash LEDs asynchronously without any use of delay()
    */
    
    if (!active) return; // LED has no active program, continue with next iteration

    currentMillis = millis();
    
    if ( currentMillis - timestamp >= on_time ) {
        digitalWrite(pin, HIGH);
        noTone(BUZZZER_PIN);
        active=0;
        return;
    }
}