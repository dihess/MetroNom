/* intr_handling.h */

#include "globals.h"


void initSemaphores();
void IRAM_ATTR rotaryEncoderISR();
void IRAM_ATTR button0ISR();
void IRAM_ATTR button5ISR();
void rotaryReaderTask(void* pArgs);
void button0ReaderTask(void* pArgs);
void button5ReaderTask(void* pArgs);
