// Global.cpp
#include <Arduino.h>
#include "Global.h"
/** \brief include all global variable definition that must be used in separate files
*/


volatile bool serialPCFlag = false;
volatile bool timerFlag = false;
volatile uint8_t evInputCode;