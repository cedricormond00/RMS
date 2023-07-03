
#ifndef BATTERY_H
#define BATTERY_H

#include "States.h"

/** 
 * @brief Initialisation sequence to ensure power supply (battery and mains) is suitable
 * 
 * If the battery is either
 * - not connected
 * - critical energy level (less then 5%) and the RMS is not connected to mains, 
 * stop the intialisation, and updates the rmsClass
 * 
 * Also sets the powerStruct values (
 * - is stable power supply
 * - battery charge status
 * - battery voltage,
 * in the rms class
 * @param rms Class
 * @return bool of battery initialisation success
*/
bool Battery_runInitSequence(rmsClass& rmsClassArg);






float Battery_getBatteryVoltage(void);
uint8_t Battery_getBatteryPercentage(float batteryVoltage);

bool Battery_getIsStablePowerSupply();
uint8_t Battery_getChargeStatus();





#endif
