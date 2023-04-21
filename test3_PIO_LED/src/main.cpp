#include <Arduino.h>
#include "LED.h"
#include "constant.h"
/*
  Blink
  Turns on all LEDs, then off, repeatedly.
 
  This example code is in the public domain.
  test test 3
 */

// if you want to declare an array of unsigned int: make sure to include the [] after the variable name
//unsigned int array_allLEDpin[] = {blueLed_pin, redLed_pin, greenLed_pin, yellowLed_pin, orangeLed_pin};
unsigned int array_allLEDpin[] = m_array_allLEDpin;
unsigned int arraySizeAllLed = sizeof(array_allLEDpin) / sizeof(array_allLEDpin[0]);
// the setup routine runs once when you press reset:


void setup() {                
  // initialize the digital pin as an output.
  // multipin_pinMode(n_allLeds, 1);     
  // digitalWrite(n_blueLed_pin, OUTPUT);
  // digitalWrite(n_redLed_pin, OUTPUT);
  // digitalWrite(n_greenLed_pin, OUTPUT);
  // digitalWrite(n_yellowLed_pin, OUTPUT);
  // digitalWrite(n_orangeLed_pin, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  while (!Serial);
  Serial.println("setup begin");
  pinMode(blueLed_pin, OUTPUT);
  for (unsigned int thisPin = 1; thisPin < arraySizeAllLed; thisPin++) {
    pinMode(array_allLEDpin[thisPin], OUTPUT);
    // Serial.println(sprintf("pin %/n", thisPin));
    Serial.print("setup: pin");
    Serial.println(array_allLEDpin[thisPin]);
    
  }
}
// the loop routine runs over and over again forever:
void loop() {
  multipin_HL(array_allLEDpin, arraySizeAllLed, 1);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  multipin_HL(array_allLEDpin, arraySizeAllLed, 0);    // turn the LED off by making the voltage LOW
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


