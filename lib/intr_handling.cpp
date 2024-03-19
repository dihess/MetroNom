#include "intr_handling.h"


#define ERROR
#define INFO
// #define DEBUG

// semaphore that will be used for reading of the rotary encoder 
static SemaphoreHandle_t rotaryISRSemaphore;
static SemaphoreHandle_t button0ISRSemaphore;
static SemaphoreHandle_t button5ISRSemaphore;

void initSemaphores() {
    rotaryISRSemaphore = xSemaphoreCreateBinary();
    button0ISRSemaphore = xSemaphoreCreateBinary();
    button5ISRSemaphore = xSemaphoreCreateBinary();
}

// Rotary encoder with MCP23017 routine
void IRAM_ATTR rotaryEncoderISR() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(rotaryISRSemaphore, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR ();
    }
}

// Int handler for rotary encoder button switches which are directly connect
void IRAM_ATTR button0ISR() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(button0ISRSemaphore, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR ();
    }
}

// Int handler for rotary encoder button switches which are directly connect
void IRAM_ATTR button5ISR() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(button5ISRSemaphore, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR ();
    }
}


void rotaryReaderTask(void* pArgs) {
  (void)pArgs;

  unsigned short int states;
  byte pin;

  mcp.readGPIOAB(); // clear MCP interrupt

  #ifdef DEBUG
  Serial.println("[rotaryReaderTask] Started rotary reader task.");
  #endif

  while(true) {
    if (xSemaphoreTake(rotaryISRSemaphore, portMAX_DELAY) == pdPASS) {
      pin = mcp.getLastInterruptPin();
      states = mcp.getCapturedInterrupt();
      #ifdef DEBUG
      Serial.print("[rotaryReaderTask] Received interrupt on pin: ");
      Serial.print(pin);
      Serial.print(" states=0b");
      Serial.println(states, 2);
      #endif
      if ( pin != 255 ) {
        for (byte i=0; i<numEncoders; i++) {
          if (encoders[i].feedEvents(pin, states))
            break;
        }
      }
    }
  }
}


void button0ReaderTask(void* pArgs) {
  (void)pArgs;

  unsigned long int last_millis = millis();

  #ifdef DEBUG
  Serial.println("[button0ReaderTask] Started button reader task.");
  #endif

  while(true) {
    if (xSemaphoreTake(button0ISRSemaphore, portMAX_DELAY) == pdPASS) {
      if ( millis() - last_millis > 250) { // debounce
        encoders[0].feedExternalSwitchEvent();
        #ifdef DEBUG
        Serial.println("[button0ReaderTask] Button 0 push detected");
        #endif
        last_millis = millis();
      }
    }
  }
}

void button5ReaderTask(void* pArgs) {
  (void)pArgs;

  unsigned long int last_millis = millis();

  #ifdef DEBUG
  Serial.println("[button5ReaderTask] Started button reader task.");
  #endif

  while(true) {
    if (xSemaphoreTake(button5ISRSemaphore, portMAX_DELAY) == pdPASS) {
      if ( millis() - last_millis > 300) { // debounce
        encoders[5].feedExternalSwitchEvent();
        #ifdef DEBUG
        Serial.println("[button5ReaderTask] Button 5 push detected");
        #endif
        last_millis = millis();
      }
    }
  }
}