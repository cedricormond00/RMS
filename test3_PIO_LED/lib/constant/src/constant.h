// constant.h
#ifndef CONSTANT_H // include guard
#define CONSTANT_H


// pin assignment
#define blueLed_pin 21
#define redLed_pin 0
#define greenLed_pin 1
#define yellowLed_pin 2
#define orangeLed_pin 3

#define m_array_allLEDpin {blueLed_pin, redLed_pin, greenLed_pin, yellowLed_pin, orangeLed_pin}

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
// #define arraySizeAllLed (sizeof(m_array_allLEDpin) / sizeof(m_array_allLEDpin[0]))

#endif