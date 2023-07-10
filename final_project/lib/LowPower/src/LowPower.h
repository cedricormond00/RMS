// Timer.h
#ifndef LOWPOWER_H // include guard
#define LOWPOWER_H

#include <Arduino.h>

void LP_goToLowPowerConsumption(rmsClass& rmsClassArg, RTCZero& rtcClassArg, volatile uint8_t* triggeredInputEvent);

void LP_setupURAInterrupt();

void LP_callbackURA();

void LP_goToDeepSleep(rmsClass& rmsClassArg, ConfigurationStruct& cfgStructArg);

void LP_callbackDeepSleep();


#endif