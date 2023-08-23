// Constant.h
#ifndef CONSTANT_H // include guard
#define CONSTANT_H

#include <stdint.h>

// global variables
// #ifndef bp // include guard
// define the variable as extern here (compiler knows this variable is declared elsewhere)
// this will be shared across all files which include this header file
// extern volatile bool bp; // true <=> button press
// #endif

// RMS number
#define RMS_ID 20
#define PINNUMBER ""



// SMS HW Window
#define SMS_HW_BUP 1200

// pin assignment
#define BLUELED_PIN 21
#define REDLED_PIN 0
#define GREENLED_PIN 1
#define YELLOWLED_PIN 2
#define ORANGELED_PIN 3

#define BUTTON_PIN 5

#define SD_CHIPSELECT 4 // pin for SD card commmunication


// I2C
#define EZO_ADDRESS 98               //default I2C ID number for EZO ORP Circuit.

// event input 
// codes used to identify what function must be performed
#define WM_INPUTBIT 0b1
#define BUP_INPUTBIT 0b10
#define URA_INPUTBIT 0b100
#define HB_INPUTBIT 0b1000
#define URA_WAIT_INPUTBIT 0b10000000 // to inform the time has not yet come to go back to sleep




//BUP
/** \brief macro to set the threshold for the power supply management
 * \remarks: can tune these parameters for some slightly different resutls
*/
#define CRITICAL_EL_LIMIT 5
#define LOW_EL_LIMIT 20






#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))


#endif