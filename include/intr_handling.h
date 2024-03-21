/* intr_handling.h */

#pragma once
#include "globals.h"

void initSemaphores();
void IRAM_ATTR rotaryEncoderISR();
void rotaryReaderTask(void* pArgs);
