#include "LEDControl.h"

LEDcontrol::LEDcontrol(uint8_t _led_pin, uint8_t _speaker_pin) {
    active = 0; // 0 = no program
    on_time = 0; // led on [ms]
    timestamp = millis();
    led_pin = _led_pin; // ESP32 pin number for LED
    speaker_pin = _speaker_pin; // ESP32 pin number for speaker outpu

    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, HIGH); // HIGH => LED off
}

LEDcontrol::~LEDcontrol() {}

void LEDcontrol::flash(unsigned short int _on_time, int _pitch) {
    on_time = _on_time;
    pitch = _pitch;

    timestamp = millis();
    digitalWrite(led_pin, LOW); // LOW => LED on
    tone(speaker_pin, pitch);

    active = 1;
    return;
}


void LEDcontrol::on() {
    digitalWrite(led_pin, LOW); // LOW => LED on
    return;
}

void LEDcontrol::off() {
    digitalWrite(led_pin, HIGH); // HIGH => LED off
    return;
}

void LEDcontrol::update() {
     /*
    Call this function in loop() to flash LEDs asynchronously without any use of delay()
    */
    
    if (!active) return; // LED has no active program, continue with next iteration

    currentMillis = millis();
    
    if ( currentMillis - timestamp >= on_time ) {
        digitalWrite(led_pin, HIGH);
        noTone(speaker_pin);

        active=0;
        return;
    }
}