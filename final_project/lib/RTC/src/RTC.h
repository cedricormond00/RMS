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

/**
 * @brief Sets up the heartbeat (HB) based on the current time and configuration.
 *
 * This function calculates the next heartbeat epoch time based on the current time and the provided configuration.
 * It uses the RTC (Real-Time Clock) to retrieve the current epoch time.
 * The calculated heartbeat epoch time is stored in the global variable `hbEPochTime`.
 *
 * @param cfgStructArg The configuration structure containing the heartbeat settings.
 * @return True if the heartbeat epoch time is successfully calculated and is greater than the current time, false otherwise.
 */
bool RTC_setUpHB(ConfigurationStruct cfgStructArg);

/**
 * @brief Finds the epoch time for the next heartbeat (HB) based on the current epoch time and configuration.
 *
 * This function calculates the epoch time for the next heartbeat based on the current epoch time and the provided configuration.
 * To do so, it adjusts the current epoch time by adding the heartbeat interval in seconds if the current time is after the specified target hour.
 * Then, it sets the target hour in the resulting date and converts it to epoch time.
 * The calculated epoch time for the next heartbeat is returned.
 *
 * @param currentEPochTime The current epoch time.
 * @param cfgStructArg The configuration structure containing the heartbeat settings.
 * @return The epoch time for the next heartbeat.
 */
uint32_t RTC_findNextHBEPochTime(uint32_t currentEPochTime, ConfigurationStruct cfgStructArg);



void RTC_printTime(RTCZero& rtcClassArg);



uint32_t RTC_updateHBEPochTime(uint32_t hbEPochTime, ConfigurationStruct cfgStructArg);



// void RTC_getTimeInText(uint32_t ePochTime, char* buf);



#endif
