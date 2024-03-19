#include "config.h"
#include "globals.h"
#include "pitches.h"
// #include <SPI.h>
// #include <WiFi.h>
// #include <String.h>
// #include "intr_handling.h"


int8_t ledPin_1 = 33;
int8_t ledPin_2 = 32;
int8_t ledPin_3 = 23;
int8_t ledPin_4 = 22;

unsigned int bpm = 250;
unsigned long bpm_ms = (60.0/bpm)*1000; // BPM in ms umgerechnet

LEDcontrol led1(ledPin_1);
LEDcontrol led2(ledPin_2);
LEDcontrol led3(ledPin_3);
LEDcontrol led4(ledPin_4);

#define BUZZZER_PIN  2 // ESP32 pin GPIO18 connected to piezo buzzer

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
int melody_1[] = {
  NOTE_A4
};

int noteDurations_1[] = {
  4
};

void setup() {
  Serial.begin(115200);

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZZER_PIN, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZZER_PIN);
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
     int noteDuration = 1000 / noteDurations[0];

    switch (beat) {
      case 1:
        led1.flash(50);
        break;
      case 2:
        led2.flash(20);
        break;
      case 3:
        led3.flash(20);
        break;
      case 4:
        led4.flash(20);   
        beat = 0; 
        break;
    }
    beat++; // beat = beat + 1

    // Serial.print("LED an");
    // digitalWrite(ledPin, HIGH); 
    // led1.on();
    // delay(500);
    
    // Serial.println(" LED aus");
    //digitalWrite(ledPin, LOW);
    // led1.off();
    last_millis = millis();
  }
}