// LED.h
#ifndef LED_H // include guard
#define LED_H
// #pragma once

// #include <Arduino.h>

//function prototypes
void multipin_pinMode(unsigned int pin_array[], unsigned int arraySize, unsigned int mode);
//void multipin_pinMode_s(unsigned int (&pin_array)[], unsigned int mode);

void multipin_HL(unsigned int pin_array[], unsigned int arraySize, unsigned int status);

void ToggleLED(unsigned int pin);

#endif