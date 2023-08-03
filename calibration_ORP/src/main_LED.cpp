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
unsigned int AllLedArray_length = ARRAY_LENGTH(allLEDpin_array);
// unsigned int AllLedArray_length = (array_allLEDpin) / sizeof(array_allLEDpin[0]);


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Setup begin");

  // unsigned int *arraypx0; //pointer to the first eleemnt of the array
  // arraypx0 = &allLEDpin_array[0]; // adress of the first array element
  // Serial.print("First array element : ");
  // Serial.println(*arraypx0); // print the content at address pointed to by arraypx0
  // unsigned int *arraypx = arraypx0 +2;//sizeof(*arraypx0); //pointer to the second element in the address
  // Serial.print("Second array element : ");
  // Serial.println(*arraypx);
  // unsigned int *arraypx = arraypx0 - sizeof(allLEDpin_array[0]); //pointer to the array
  // Serial.print("Array : ");
  // Serial.println(*arraypx);



  // initialize the digital pin as an output.
  //multipin_pinMode(allLEDpin_array, ARRAY_LENGTH(allLEDpin_array), 1);
  multipin_pinMode(allLEDpin_array, AllLedArray_length, 1);
}

void loop()
{
  // if you want to declare an array of unsigned int: make sure to include the [] after the variable name
  // unsigned int array_allLEDpin[] = m_array_allLEDpin;
  // unsigned int AllLedArray_length = sizeof(array_allLEDpin) / sizeof(array_allLEDpin[0]);

  multipin_HL(allLEDpin_array, AllLedArray_length, 1);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  multipin_HL(allLEDpin_array, AllLedArray_length, 0);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
  Serial.println("loop: pin");
  
}