// constant.h
#ifndef CONSTANT_H // include guard
#define CONSTANT_H


// pin assignment
#define BLUELED_PIN 21
#define REDLED_PIN 0
#define GREENLED_PIN 1
#define YELLOWLED_PIN 2
#define ORANGELED_PIN 3
#define BUTTON_PIN 5

#define m_array_allLEDpin {BLUELED_PIN, REDLED_PIN, GREENLED_PIN, YELLOWLED_PIN, ORANGELED_PIN}

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
// #define arraySizeAllLed (sizeof(m_array_allLEDpin) / sizeof(m_array_allLEDpin[0]))

#endif