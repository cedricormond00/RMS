// RTC.h
#ifndef RTC_H // include guard
#define RTC_H



#include <RTCZero.h>
#include "Configuration.h"

/**
 * @brief initialisaes both RTC objects
 * @details Also 
 * - sets the internal RTC to the correct time, using the external RTC
 * - enables the alarm from the internal RTC
 * - attaches the crorect call back function ti exectue when an alarm is raised
 * @warning requires that both RTC objects are declared as global variables
 * @param none
 * @return bool of FRTC initialisation success 
*/
bool RTC_init(void);

/**
 * @brief updates internal RTC using external RTC
 * @remark to reduce costs: could simply use the nb.getTime to get ther same results
 * @warning Assumes the rtcds3231 has already been initialised
 * @param none
 * @return none
*/
void RTC_updateInternalRTCToCurrentTime(void);

/** 
 * \brief callback function on RTC alarm match
*/
void RTC_callbackAlarmMatch();

void RTC_print2digits(int number);
void RTC_printTime(RTCZero& rtcClassArg);



bool RTC_setUpHB(ConfigurationStruct cfgStructArg);
uint32_t RTC_findNextHBEPochTime(uint32_t currentEPochTime, ConfigurationStruct cfgStructArg);
uint32_t RTC_updateHBEPochTime(uint32_t hbEPochTime, ConfigurationStruct cfgStructArg);



// void RTC_getTimeInText(uint32_t ePochTime, char* buf);



#endif
