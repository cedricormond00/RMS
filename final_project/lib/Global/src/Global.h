// Global.h


#ifndef GLOBAL_H
#define GLOBAL_H

#include <RTCZero.h>
#include <RTClib.h>
#include <MKRNB.h>

/** \brief include all global variable declaration that must be used in separate files
 * \remark the global variable declaration is done in Global.c
*/


// Define the RTCZero object
extern RTCZero rtc;
// Define the RTC_DS3231 object
extern RTC_DS3231 rtcDS3231;

// initialize the library instance
extern NB nbAccess;
extern NB_SMS sms;

// variable to keep track of the epochtime when an alarm match occurs between the internal clock and the set alarm time
extern uint32_t alarmMatchEPochTime;
// variable to keep track of the next hbEpochTime
extern uint32_t hbEPochTime;

// variable to keep track of which event was triggered
extern volatile uint8_t triggeredInputEvent;
// millis value when the external interrupt (Button press) occured
//TODO: deal with overflow from millis
extern volatile uint32_t millisOnExternalWakeUp;
// millis value required to keep track of when to toggle the yellow led
extern volatile uint32_t previousMillis;




#endif // GLOBALS_H