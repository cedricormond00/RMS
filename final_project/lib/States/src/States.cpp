#include <Arduino.h>

#include "States.h"



rmsClass::rmsClass(){
    _rmsState = INIT;
    _sleepPeriod = _UWQSleepPeriod; //or 0 to start with
}

// _rmsState
void rmsClass::set_rmsState(RMSState newState){
    _previousRMSState = _rmsState;// store the previous state
    _rmsState = newState; // store the current updated state
}

RMSState rmsClass::get_rmsState(){
    return _rmsState;
}

RMSState rmsClass::get_previousRMSState(){
    return _previousRMSState;
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

// _sleepPeriod
uint32_t rmsClass::get_sleepPeriod(){
    return _sleepPeriod;
}

void rmsClass::set_sleepPeriod(){
    switch (_rmsState)
    {
    case (INIT):
        _sleepPeriod = 0;
        break;
    
    case (SWQ):
        _sleepPeriod = _SWQSleepPeriod;
        break;
    case (UWQ):
        _sleepPeriod = _UWQSleepPeriod;
        break;
    case (FWQ):
        _sleepPeriod = _FWQSleepPeriod;
    default:
        break;
    }

}

 uint8_t rmsClass::get_sleepPeriod(RMSState anyState){
    switch (anyState)
    {
    case (INIT):
        return 0;
    
    case (SWQ):
        return _SWQSleepPeriod;

    case (UWQ):
        return _UWQSleepPeriod;

    case (FWQ):
        return _FWQSleepPeriod;

    default:
        break;
    }
 }