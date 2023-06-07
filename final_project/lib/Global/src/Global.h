// Global.h


#ifndef GLOBAL_H
#define GLOBAL_H

#include <RTCZero.h>
#include <RTClib.h>

/** \brief include all global variable declaration that must be used in separate files
 * \remark the global variable declaration is done in Global.c
*/

extern volatile bool serialPCFlag;
extern volatile uint8_t evInputCode;
//timer
extern volatile bool timerFlag;         // variable to keep track of time increment according to timeIncrement
extern volatile uint32_t timeIncrement; //in ms
extern volatile char timeIncrementType;

extern uint32_t timeIncrementS;
extern volatile uint32_t WMTC; // Water motnitoring timer counter

extern volatile uint8_t triggeredInputEvent;
extern volatile unsigned long millisOnExternalWakeUp;
extern volatile unsigned long previousMillis;

extern char dataFileName[13];

extern RTCZero rtc;
extern uint32_t alarmMatchEPochTime;

extern RTC_DS3231 rtcDS3231;
extern uint32_t hbEPochTime;


#endif // GLOBALS_H