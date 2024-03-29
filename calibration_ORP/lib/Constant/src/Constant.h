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



// SMSWindow
// #define SMS_HW_WQ 20
// #define SMS_HW_URA 20
#define SMS_HW_BUP 1200

// pin assignment
#define BLUELED_PIN 21
#define REDLED_PIN 0
#define GREENLED_PIN 1
#define YELLOWLED_PIN 2
#define ORANGELED_PIN 3

#define BUTTON_PIN 5

#define SD_CHIPSELECT 4


// I2C
#define EZO_ADDRESS 98               //default I2C ID number for EZO ORP Circuit.
// #define RTCDS3231_Address 

// event input 
#define WM_INPUTBIT 0b1
#define BUP_INPUTBIT 0b10
#define URA_INPUTBIT 0b100
#define HB_INPUTBIT 0b1000
#define URA_WAIT_INPUTBIT 0b10000000 // to inform the time has not yet come to go back to sleep

// // ATTENTION: refence must be a leap year!!
// #define DEFAULT_EPOCHTIME 1577836800 //01/01/2020 00:00:00
// #define DEFAULT_INITSLEEPPERIOD 10 // time at which the frist reading will occur

// ML
// #define LOGIT_THRESHOLD 400

//BUP
/** \brief macro to set the threshold for the power supply management
 * \remarks: can tune these parameters for some slightly different resutls
*/
#define CRITICAL_EL_LIMIT 5
#define LOW_EL_LIMIT 20

// //URA
// #define URA_PRESS_DURATION 3000


// array assignment
// #define m_array_allLEDpin {BLUELED_PIN, REDLED_PIN, GREENLED_PIN, YELLOWLED_PIN, ORANGELED_PIN}

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
// #define arraySizeAllLed (sizeof(m_array_allLEDpin) / sizeof(m_array_allLEDpin[0]))



#endif