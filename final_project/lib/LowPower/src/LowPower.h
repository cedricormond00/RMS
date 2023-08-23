// Timer.h
#ifndef LOWPOWER_H // include guard
#define LOWPOWER_H

#include <Arduino.h>

/** \brief sends the arduino to sleep to save power.
 * It checks that it may go to sleep 
 * 
*/
void LP_goToLowPowerConsumption(rmsClass& rmsClassArg, RTCZero& rtcClassArg, volatile uint8_t* triggeredInputEvent);

/** \brief attaches the URA interrupt, so that the device gets woken up ipon user button press
*/
void LP_setupURAInterrupt();

/** \brief callback function when the button has been pressed.
 * This will trigger the global variable "triggeredInputEvent" with the appropriate flag "URA_INPUTBIT" to inform the software to potentially perfomr an URA function, if the device is pressed for long enough.
*/
void LP_callbackURA();

/** \brief sends the device to deepsleep when then battery is too low
*/
void LP_goToDeepSleep(rmsClass& rmsClassArg, ConfigurationStruct& cfgStructArg);

/** \brief Wkaes up the device from its deepsleep if required
 * This function may require a bit of reworking
*/
void LP_callbackDeepSleep();


#endif