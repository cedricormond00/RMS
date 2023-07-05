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

/**
 * @brief Updates the epoch time for the next Heartbeat (HB) event.
 * 
 * @param hbEPochTime The epoch time of the current Heartbeat event.
 * @param cfgStructArg The configuration settings for the system.
 * 
 * @return The updated epoch time for the next Heartbeat event.
 * 
 * @details This function takes the current epoch time of the Heartbeat event and
 * adds the configured Heartbeat interval in hours to calculate the epoch time for
 * the next Heartbeat event. The Heartbeat interval determines how often the Heartbeat
 * event occurs.
 * 
 * @note The function assumes that the Heartbeat interval in the configuration settings
 * is properly set and represents the desired time interval in hours between Heartbeat events.
 * 
 * @warning The function relies on the correctness of the current Heartbeat epoch time and
 * the Heartbeat interval in the configuration settings. Ensure that these values are accurate
 * to obtain the correct epoch time for the next Heartbeat event.
 */
uint32_t RTC_updateHBEPochTime(uint32_t hbEPochTime, ConfigurationStruct cfgStructArg);


void RTC_printTime(RTCZero& rtcClassArg);






// void RTC_getTimeInText(uint32_t ePochTime, char* buf);



#endif
