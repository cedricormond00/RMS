// Timer.h
#ifndef LOWPOWER_H // include guard
#define LOWPOWER_H

#include <Arduino.h>

void LP_goToLowPowerConsumption(rmsClass& rmsClassArg, RTCZero& rtcClassArg, uint8_t inputEventCode, uint32_t intendedWakePeriod);

void LP_setupURAInterrupt();

void LP_callbackURA();

#endif