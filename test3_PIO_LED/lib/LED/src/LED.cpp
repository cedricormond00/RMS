#include <Arduino.h>
#include "LED.h"

void multipin_HL(unsigned int n_pinAllLEDs[], unsigned int status)
{
    unsigned int n_arraySize = sizeof(n_pinAllLEDs) / sizeof(n_pinAllLEDs[0]);
    for (unsigned int i = 0; i < n_arraySize; i++) {
        digitalWrite(n_pinAllLEDs[i], status);
  }
}
