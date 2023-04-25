#include <Arduino.h>
#include "LED.h"
#include "constant.h"
// #include "template.h"

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

void ToggleLED(unsigned int pin){
  digitalWrite(pin, ~digitalRead(pin));
}

// void multipin_pinMode_s(unsigned int (&pin_array)[], unsigned int mode)
// {
//   unsigned int *arraypx0; //pointer to the first eleemnt of the array
//   arraypx0 = &allLEDpin_array[0]; // adress of the first array element
//   Serial.print("First array element : ");
//   Serial.println(*arraypx0); // print the content at address pointed to by arraypx0
//   unsigned int *arraypx = arraypx0 + 1;//sizeof(*arraypx0); //pointer to the second element in the address
//   Serial.print("Second array element : ");
//   Serial.println(*arraypx);
//   unsigned int arraySize = sizeof(pin_array) / sizeof(pin_array[0]);
//   //size_t arraySize = array_length(pin_array);

//   for (unsigned int currPin = 0; currPin < arraySize; currPin++) 
//   {
//     pinMode(currPin, mode);
//   }
// }