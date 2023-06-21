// RTC.h
#ifndef RTC_H // include guard
#define RTC_H



#include <RTCZero.h>
#include "Configuration.h"

bool RTC_init(void);
void RTC_updateInternalRTCToCurrentTime(void);

void RTC_print2digits(int number);
void RTC_printTime(RTCZero& rtcClassArg);

/** \brief function called RTC matches set alarm time
 * \remarks - currently deals with alarm match upon WM request.
 * - TODO: future: needs to deal with Hb time alaram raised
*/
void RTC_callbackAlarmMatch();

bool RTC_setUpHB(ConfigurationStruct cfgStructArg);
uint32_t RTC_findNextHBEPochTime(uint32_t currentEPochTime, ConfigurationStruct cfgStructArg);
uint32_t RTC_updateHBEPochTime(uint32_t hbEPochTime, ConfigurationStruct cfgStructArg);



void RTC_getTimeInText(uint32_t ePochTime, char* buf);



#endif
