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
volatile bool bp = false; // true <=> button press

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
  Serial.println("before interrupt setup");
  RegisterView();



  // Interrupts_init();
  // Serial.println("After Arduino interrupt setup");
  // RegisterView();

  Interrupts_init_LL();
  Serial.println("After Low Level interrupt setup");
  RegisterView();
  Serial.println("Setup complete");

}

void loop()
{ 
  if (false){
    Serial.print("button state: ");
    Serial.println(digitalRead(BUTTON_PIN));
    Serial.print("LED pin state: ");
    Serial.println(digitalRead(ORANGELED_PIN));
    Serial.print("bp :");
    Serial.println(bp);
    Serial.println();

    delay(500);
  }
  if (bp) {
    Serial.println("button pressed");
    Serial.print("bp:");
    Serial.println(bp);

    //digitalWrite(ORANGELED_PIN, LOW);
    ToggleLED(ORANGELED_PIN);
    bp = false;
  }
  if (false){
    Serial.print("flag state:");
    Serial.println(EIC->INTFLAG.bit.EXTINT11);
    Serial.println(EIC->INTFLAG.reg);
  }

  if(EIC->INTFLAG.bit.EXTINT11){
        // clear the interrupt flag
        EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT11;
        bp = true;
    }
}