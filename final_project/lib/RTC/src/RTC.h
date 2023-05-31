// RTC.h
#ifndef RTC_H // include guard
#define RTC_H

#include <Arduino.h>

#include <RTCZero.h>

void RTC_init(RTCZero& rtc);
void RTC_print2digits(int number);
void RTC_printTime(RTCZero& rtcClassArg);

/** \brief function called RTC matches set alarm time
 * \remarks - currently deals with alarm match upon WM request.
 * - TODO: future: needs to deal with Hb time alaram raised
*/
void RTC_callbackAlarmMatch();

void RTC_getTimeInText(uint32_t ePochTime, char* buf);



#endif
