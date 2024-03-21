#include <Arduino.h>
#include "pitches.h"
#include "config.h"
#include "globals.h"
#include "RotaryEncoder.h"
#include "intr_handling.h"

#define DEBUG

// #include <SPI.h>
// #include <WiFi.h>
// #include <String.h>

unsigned int bpm = 250;
unsigned long bpm_ms = (60.0/bpm)*1000; // BPM in ms umgerechnet

LEDcontrol led1(ledPin_1, speaker_pin);
LEDcontrol led2(ledPin_2, speaker_pin);
LEDcontrol led3(ledPin_3, speaker_pin);
LEDcontrol led4(ledPin_4, speaker_pin);

RotaryEncoder encoder(enc_pinA, enc_pinB, enc_pinC, 0); 

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

  // Intialize rotary encoder
  encoder.init();

  // Settle tasks and interrupts
  initSemaphores();

  xTaskCreatePinnedToCore(rotaryReaderTask, "rotary reader", 2048, NULL, 2, NULL, 0);
  
  attachInterrupt(digitalPinToInterrupt(enc_pinA), rotaryEncoderISR, CHANGE); // encoder CLK
  attachInterrupt(digitalPinToInterrupt(enc_pinB), rotaryEncoderISR, CHANGE); // encoder DT
  attachInterrupt(digitalPinToInterrupt(enc_pinC), rotaryEncoderISR, FALLING); // encoder switch
}

void loop() {
  static unsigned long last_millis;
  static uint8_t beat = 1;

  led1.update();
  led2.update();
  led3.update();
  led4.update();

  // check for rotary encoder events
  if (encoder.isChanged() == 1) {
    bpm += encoder.getTicks() * 10;
    bpm_ms = (60.0/bpm)*1000; // BPM in ms umgerechnet
    #ifdef DEBUG
    Serial.printf("[main::loop] BPM changed to %i. bpm_ms is now: %i\n", bpm, bpm_ms);
    #endif
  }

  if (millis() - last_millis > bpm_ms ) {
    switch (beat) {
      case 1:
        led1.flash(100, NOTE_A6);
        break;
      case 2:
        led2.flash(50, NOTE_A5);
        break;
      case 3:
        led3.flash(50, NOTE_A5);
        break;
      case 4:
        led4.flash(50, NOTE_A5);   
        beat = 0; 
        break;
    }

    beat++; // beat = beat + 1
    last_millis = millis();
  }
}