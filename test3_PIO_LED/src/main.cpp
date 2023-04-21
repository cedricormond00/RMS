#include <Arduino.h>
#include "LED.h"
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
  test test 3
 */


// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
unsigned int n_blueLed_pin = 21;
unsigned int n_redLed_pin = 0;
unsigned int n_greenLed_pin = 1;
unsigned int n_yellowLed_pin = 2;
unsigned int n_orangeLed_pin = 3;
unsigned int n_allLeds[] = {n_blueLed_pin, n_redLed_pin, n_greenLed_pin, n_yellowLed_pin, n_orangeLed_pin};
unsigned int arraySize = sizeof(n_allLeds) / sizeof(n_allLeds[0]);


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
  Serial.println("pin");
  for (unsigned int thisPin = 0; thisPin < arraySize; thisPin++) {
    pinMode(n_allLeds[thisPin], OUTPUT);
    
  }
}
// the loop routine runs over and over again forever:
void loop() {
  multipin_HL(n_allLeds, 1);   // turn the LED on (HIGH is the voltage level)
  delay(2000);              // wait for a second
  multipin_HL(n_allLeds, 0);    // turn the LED off by making the voltage LOW
  delay(2000);               // wait for a second
  Serial.println("pin");
}


// void multipin_pinMode(int n_pinAllLEDs[], int status)
// {
//   for(int n_pinLED : n_pinAllLEDs)// for each element in the array
//   {
//     pinMode(n_pinLED, 1);
//   }
// }


