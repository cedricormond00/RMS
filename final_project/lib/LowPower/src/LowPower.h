// Timer.h
#ifndef LOWPOWER_H // include guard
#define LOWPOWER_H

#include <Arduino.h>

void LP_goToLowPowerConsumption(uint8_t inputEventCode, uint32_t wakeperiod);

void LP_setupURAInterrupt();

void LP_callbackURA();

#endif