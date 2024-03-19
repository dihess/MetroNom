#include "config.h"
#include "globals.h"
#include "pitches.h"
// #include <SPI.h>
// #include <WiFi.h>
// #include <String.h>
// #include "intr_handling.h"

unsigned int bpm = 250;
unsigned long bpm_ms = (60.0/bpm)*1000; // BPM in ms umgerechnet

LEDcontrol led1(ledPin_1, speaker_pin);
LEDcontrol led2(ledPin_2, speaker_pin);
LEDcontrol led3(ledPin_3, speaker_pin);
LEDcontrol led4(ledPin_4, speaker_pin);


void setup() {
  Serial.begin(115200);

  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };

  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };
  
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speaker_pin, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(speaker_pin);
  }
}

void loop() {
  static unsigned long last_millis;
  static uint8_t beat = 1;

  led1.update();
  led2.update();
  led3.update();
  led4.update();

  if (millis() - last_millis > bpm_ms ) {
    switch (beat) {
      case 1:
        led1.flash(70, NOTE_A6);
        break;
      case 2:
        led2.flash(30, NOTE_A5);
        break;
      case 3:
        led3.flash(30, NOTE_A5);
        break;
      case 4:
        led4.flash(30, NOTE_A5);   
        beat = 0; 
        break;
    }

    beat++; // beat = beat + 1
    last_millis = millis();
  }
}