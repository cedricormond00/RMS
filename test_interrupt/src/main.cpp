#include <Arduino.h>
#include "LED.h"
#include "constant.h"
/*
  button press
  button press to trigger an LED
 */

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Setup begin");

  // initialize 
  // LED digital pin as an output.
  pinMode(ORANGELED_PIN, OUTPUT);
  // button press as an input
  pinMode(BUTTON_PIN, INPUT);

}

void loop()
{
  bool btnState = digitalRead(BUTTON_PIN);
  if (btnState) 
  {
    digitalWrite(ORANGELED_PIN, HIGH);
  }
  else
  {
    digitalWrite(ORANGELED_PIN, LOW);
  }
  Serial.println(btnState);
  delay(25);
  
}