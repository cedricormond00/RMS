#include <Arduino.h>
#include "LED.h"
#include "constant.h"
/*
  Blink
  Turns on all LEDs, then off, repeatedly.
 
  This example code is in the public domain.
  test test 3
 */
unsigned int allLEDpin_array[] = m_array_allLEDpin;
unsigned int arraySizeAllLed = sizeof(allLEDpin_array) / sizeof(allLEDpin_array[0]);
// unsigned int arraySizeAllLed = (array_allLEDpin) / sizeof(array_allLEDpin[0]);


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Setup begin");

  // initialize the digital pin as an output.
  multipin_pinMode(allLEDpin_array, arraySizeAllLed, OUTPUT);

  // for (unsigned int thisPin = 0; thisPin < arraySizeAllLed; thisPin++) 
  // {
  //   pinMode(allLEDpin_array[thisPin], OUTPUT);
  //   Serial.print("setup: pin");
  //   Serial.println(allLEDpin_array[thisPin]);
  // }
}

void loop()
{
  // if you want to declare an array of unsigned int: make sure to include the [] after the variable name
  // unsigned int array_allLEDpin[] = m_array_allLEDpin;
  // unsigned int arraySizeAllLed = sizeof(array_allLEDpin) / sizeof(array_allLEDpin[0]);

  multipin_HL(allLEDpin_array, arraySizeAllLed, 1);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  multipin_HL(allLEDpin_array, arraySizeAllLed, 0);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
  Serial.println("loop: pin");
}




// void multipin_pinMode(int n_pinAllLEDs[], int status)
// {
//   for(int n_pinLED : n_pinAllLEDs)// for each element in the array
//   {
//     pinMode(n_pinLED, 1);
//   }
// }


