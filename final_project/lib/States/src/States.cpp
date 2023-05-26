#include <Arduino.h>

#include "States.h"


rmsClass::rmsClass(){
    _rmsState = INIT;
}

// _rmsState
void rmsClass::set_rmsState(RMSState newState){
    _rmsState = newState;
}

RMSState rmsClass::get_rmsState(){
    return _rmsState;
}


// _wakeUpEPochTime
uint32_t rmsClass::get_wakeUpEPochTime(){
    return _wakeUpEPochTime;
}

void rmsClass::set_wakeUpEPochTime(uint32_t new_wakeUpEPochTime){
    _wakeUpEPochTime = new_wakeUpEPochTime;
}

// _toSleepEPochTime
uint32_t rmsClass::get_toSleepEPochTime(){
    return _toSleepEPochTime;
}

void rmsClass::set_toSleepEPochTime(uint32_t new_toSleepEPochTime){
    _toSleepEPochTime = new_toSleepEPochTime;
}

