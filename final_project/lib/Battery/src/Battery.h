
#ifndef BATTERY_H
#define BATTERY_H

#include "States.h"

float Battery_getBatteryVoltage(void);
uint8_t Battery_getBatteryPercentage(uint8_t batteryVoltage);

bool Battery_getIsStablePowerSupply();
uint8_t Battery_getChargeStatus();

bool Battery_runInitSequence(rmsClass& rmsClassArg);



#endif
