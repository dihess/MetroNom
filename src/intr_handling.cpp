#include "intr_handling.h"

#define ERROR
#define INFO
#define DEBUG

// semaphore that will be used for reading of the rotary encoder 
static SemaphoreHandle_t rotaryISRSemaphore;

void initSemaphores() {
    rotaryISRSemaphore = xSemaphoreCreateBinary();
}

// Rotary encoder with MCP23017 routine
void IRAM_ATTR rotaryEncoderISR() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(rotaryISRSemaphore, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR ();
    }
}

void rotaryReaderTask(void* pArgs) {
  (void)pArgs;

  bool valueA, valueB, valueC;

  while(true) {
    if (xSemaphoreTake(rotaryISRSemaphore, portMAX_DELAY) == pdPASS) {
      #ifdef DEBUG
        digitalWrite(12,true);
        delay(1);
        digitalWrite(12,false);
      #endif
      /*
      valueA = digitalRead(enc_pinA); // read CLK
      valueB = digitalRead(enc_pinB); // read DT

      if (enc_pinC > -1) {
        valueC = digitalRead(enc_pinC); 
      } else {
        valueC = -1;
      }
      */
      encoder.feedEvents();
    }
  }
}
