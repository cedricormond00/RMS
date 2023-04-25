#include <Arduino.h>
// #include <samd21g18a.h>     // include the SAMD21 device header file
// #include <sam.h>            // include the CMSIS header file
#include "constant.h"
#include "LED.h"

#include "interrupts.h"
/*
  button press
  button press to trigger an LED
 */

// declare the variable bp here and only here for the first time
bool bp = false; // true <=> button press

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Setup begin");

  // initialize 
  // LED digital pin as an output.
  pinMode(ORANGELED_PIN, OUTPUT);
  // button press as an input
  pinMode(BUTTON_PIN, INPUT);

  // set the LED initially on
  digitalWrite(ORANGELED_PIN, HIGH);

  Interrupts_init();
}

void loop()
{  
  if (bp) {
    Serial.println("button pressed");
    //digitalWrite(ORANGELED_PIN, HIGH);
    ToggleLED(ORANGELED_PIN);
    bp = false;
  }
}