#include <Arduino.h>
#include "LED.h"
#include "constant.h"

void multipin_HL(unsigned int pin_arr[], unsigned int arraySize, unsigned int status)
{
  // unsigned int n_arraySize = sizeof(n_pinAllLEDs) / sizeof(n_pinAllLEDs[0]);
  // Serial.println(n_pinAllLEDs[2]);
  //char c_arraySize[100];
  //Serial.print(sprintf(c_arraySize, "array size %u", n_arraySize));
  // Serial.print("array size");
  // Serial.println(n_arraySize);
  for (unsigned int i = 0; i < arraySize; i++) 
  {
      digitalWrite(pin_arr[i], status);
      Serial.print("Toggle pin ");
      Serial.println(pin_arr[i]);
  }
}

void multipin_pinMode(unsigned int pin_array[], unsigned int arraySize, unsigned int mode)
{
  for (unsigned int currPin = 0; currPin < arraySize; currPin++) 
  {
    pinMode(currPin, mode);
  }
}