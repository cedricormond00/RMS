// Global.cpp
#include <Arduino.h>
#include "Global.h"
/** \brief include all global variable definition that must be used in separate files
*/


// volatile bool serialPCFlag = false;

// volatile uint8_t evInputCode = 0b0;
// // timer parameters
// volatile bool timerFlag = false;
// // how often the interrupt will trigger
// volatile uint32_t timeIncrement = 1500; //in ms // although this one shoul be a constant
// volatile char timeIncrementType = {'t'}; // either t or f
// uint32_t timeIncrementS = timeIncrement/1000; // time increment in second
// // These would be better defined in a class or a struct
// volatile uint32_t WMTC = 0 ; // Water motnitoring timer counter: incrmeenets every timeIncrement

// rtc objects instance
RTCZero rtc;
RTC_DS3231 rtcDS3231;

// initialize the library instance
NB nbAccess;
NB_SMS sms;

uint32_t alarmMatchEPochTime = 0; 
uint32_t hbEPochTime = 0;

volatile uint8_t triggeredInputEvent = 0;
volatile uint32_t millisOnExternalWakeUp = 0;
volatile uint32_t previousMillis = 0;


