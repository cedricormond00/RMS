// RTC.h
#ifndef RTC_H // include guard
#define RTC_H



#include <RTCZero.h>

void RTC_init(void);
void RTC_updateInternalRTCToCurrentTime(void);

void RTC_print2digits(int number);
void RTC_printTime(RTCZero& rtcClassArg);

/** \brief function called RTC matches set alarm time
 * \remarks - currently deals with alarm match upon WM request.
 * - TODO: future: needs to deal with Hb time alaram raised
*/
void RTC_callbackAlarmMatch();

uint32_t RTC_findNext9AMEPochTime(uint32_t currentEPochTime);
uint32_t RTC_updateHBEPochTime(uint32_t hbEPochTime);



void RTC_getTimeInText(uint32_t ePochTime, char* buf);



#endif
