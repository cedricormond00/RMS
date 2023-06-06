#include <Arduino.h>


#include "States.h"
#include "Constant.h"

// Implement the constructor for the struct
rmsClass::alarmStruct::alarmStruct(uint32_t lastAlarmSMSEPochTime_initVal = 0, 
                     uint32_t currentAlarmEPochTime_initVal = 0, 
                     uint32_t allowedIntervalBetweenSMS_initVal = 20){
    lastAlarmSMSEPochTime = lastAlarmSMSEPochTime_initVal;
    currentAlarmEPochTime = currentAlarmEPochTime_initVal;
    allowedIntervalBetweenSMS = allowedIntervalBetweenSMS_initVal;
}

rmsClass::rmsClass() : _uraStruct(0,0,SMS_HW_URA), _wmStruct(0,0,SMS_HW_WQ){
    _rmsState = INIT;
    _inputEventCode = 0b0;
    _sleepPeriod = _UWQSleepPeriod; //or 0 to start with
    _nextWakeUpEPochTime = DEFAULT_EPOCHTIME + 10;
}

// _rmsState
void rmsClass::set_rmsState(RMSState newState){
    _previousRMSState = _rmsState;// store the previous state
    _rmsState = newState; // store the current updated state
}

RMSState rmsClass::get_rmsState(){
    return _rmsState;
}


// _previousRMSState
RMSState rmsClass::get_previousRMSState(){
    return _previousRMSState;
}


// _inputEventCode
uint8_t rmsClass::get_inputEventCode(){
    return _inputEventCode;
}

void rmsClass::set_inputEventCode(uint8_t new_inputEventCode){
    _inputEventCode = new_inputEventCode;
}


// _wakeUpEPochTime
uint32_t rmsClass::get_wakeUpEPochTime(){
    return _wakeUpEPochTime;
}

void rmsClass::set_wakeUpEPochTime(uint32_t new_wakeUpEPochTime){
    _wakeUpEPochTime = new_wakeUpEPochTime;
}

// _wmWakeUpEPochTime
uint32_t rmsClass::get_wmWakeUpEPochTime(){
    return _wmWakeUpEPochTime;
}

void rmsClass::set_wmWakeUpEPochTime(uint32_t new_wmWakeUpEPochTime){
    _wmWakeUpEPochTime = new_wmWakeUpEPochTime;
}


// _nextWakeUpEPochTime
uint32_t rmsClass::get_nextWakeUpEPochTime(){
    return _nextWakeUpEPochTime;
}

void rmsClass::set_nextWakeUpEPochTime(uint32_t new_nextWakeUpEPochTime){
    _nextWakeUpEPochTime = new_nextWakeUpEPochTime;

}


// _wmReadEPochTime
uint32_t rmsClass::get_wmReadEPochTime(){
    return _wmReadEPochTime;
}

void rmsClass::set_wmReadEPochTime(uint32_t new_wmReadEPochTime){
    _wmReadEPochTime = new_wmReadEPochTime;
}

// _toSleepEPochTime
uint32_t rmsClass::get_toSleepEPochTime(){
    return _toSleepEPochTime;
}

void rmsClass::set_toSleepEPochTime(uint32_t new_toSleepEPochTime){
    _toSleepEPochTime = new_toSleepEPochTime;
}


// _orpReading
float rmsClass::get_orpReading(){
    return _orpReading;
}
void rmsClass::set_orpReading(float new_orpReading){
    _orpReading = new_orpReading;
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
    return 0;
 }


 // alarm raising


//  initialize each instance of the private struct using the constructor and provide the desired values for each instance:
// rmsClass::rmsClass() : _uraStruct(0,0,20){}

void rmsClass::set_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg, uint32_t new_lastAlarmSMSEPochTime){
    alarmStructArg.lastAlarmSMSEPochTime = new_lastAlarmSMSEPochTime;
}

uint32_t rmsClass::get_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg){
    return alarmStructArg.lastAlarmSMSEPochTime;
}

void rmsClass::set_currentAlarmEPochTime(alarmStruct& alarmStructArg, uint32_t new_currentAlarmEPochTime){
    alarmStructArg.currentAlarmEPochTime = new_currentAlarmEPochTime;
}

uint32_t rmsClass::get_currentAlarmEPochTime(alarmStruct& alarmStructArg){
    return alarmStructArg.currentAlarmEPochTime;
}


uint32_t rmsClass::get_allowedIntervalBetweenSMS(alarmStruct& alarmStructArg){
    return alarmStructArg.allowedIntervalBetweenSMS;
}

//URA
void rmsClass::set_URAlastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime){
    set_lastAlarmSMSEPochTime(_uraStruct, new_lastAlarmSMSEPochTime);
}

uint32_t rmsClass::get_URAlastAlarmSMSEPochTime(){
    return get_lastAlarmSMSEPochTime(_uraStruct);
}

void rmsClass::set_URAcurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime){
    set_currentAlarmEPochTime(_uraStruct, new_currentAlarmEPochTime);
}

uint32_t rmsClass::get_URAcurrentAlarmEPochTime(){
    return get_currentAlarmEPochTime(_uraStruct);
}

uint32_t rmsClass::get_URAallowedIntervalBetweenSMS(){
    return get_allowedIntervalBetweenSMS(_uraStruct);
}

bool rmsClass::ura_canSendSMS(uint32_t new_currentAlarmEPochTime){
    bool canSendSMS = true;
    set_URAcurrentAlarmEPochTime(new_currentAlarmEPochTime);
    if (get_URAcurrentAlarmEPochTime()-get_URAlastAlarmSMSEPochTime() > get_URAallowedIntervalBetweenSMS()){
        set_URAlastAlarmSMSEPochTime(get_URAcurrentAlarmEPochTime());
    }
    else {
        canSendSMS = false;
    }
    return canSendSMS;
}


//WM
void rmsClass::set_wmLastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime){
    set_lastAlarmSMSEPochTime(_wmStruct, new_lastAlarmSMSEPochTime);
}

uint32_t rmsClass::get_wmLastAlarmSMSEPochTime(){
    return get_lastAlarmSMSEPochTime(_wmStruct);
}

void rmsClass::set_wmCurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime){
    set_currentAlarmEPochTime(_wmStruct, new_currentAlarmEPochTime);
}

uint32_t rmsClass::get_wmCurrentAlarmEPochTime(){
    return get_currentAlarmEPochTime(_wmStruct);
}

uint32_t rmsClass::get_wmAllowedIntervalBetweenSMS(){
    return get_allowedIntervalBetweenSMS(_wmStruct);
}

bool rmsClass::wm_canSendSMS(uint32_t new_currentAlarmEPochTime){
    bool canSendSMS = true;
    set_wmCurrentAlarmEPochTime(new_currentAlarmEPochTime);
    if (get_wmCurrentAlarmEPochTime()-get_wmLastAlarmSMSEPochTime() > get_wmAllowedIntervalBetweenSMS()){
        set_wmLastAlarmSMSEPochTime(get_wmCurrentAlarmEPochTime());
    }
    else {
        canSendSMS = false;
    }
    return canSendSMS;
}


// History
void rmsClass::set_stateHistoryCount(RMSState stateOfInterest, uint8_t countState){
    switch(stateOfInterest){
        case(SWQ):
            _stateHistoryStruct.n_SWQ = countState;
        case(UWQ):
            _stateHistoryStruct.n_UWQ = countState;
        case(FWQ):
            _stateHistoryStruct.n_FWQ = countState;
    }
}
uint8_t rmsClass::get_stateHistoryCount(RMSState stateOfInterest){
    switch(stateOfInterest){
        case(SWQ):
            return _stateHistoryStruct.n_SWQ;
        case(UWQ):
            return _stateHistoryStruct.n_UWQ;
        case(FWQ):
            return _stateHistoryStruct.n_FWQ;
        default:
            Serial.println("Not a valid state");
            return 255;
    }
}
uint8_t rmsClass::get_totalStateChanges(){
    updateTotalStateChanges();
    return _stateHistoryStruct.n_meas;
}

uint8_t rmsClass::updateTotalStateChanges(){
    _stateHistoryStruct.n_meas = _stateHistoryStruct.n_SWQ +
                                _stateHistoryStruct.n_UWQ +
                                _stateHistoryStruct.n_FWQ;
    return _stateHistoryStruct.n_meas;
}


void rmsClass::set_stateHistoryPercentage(RMSState stateOfInterest){
    switch(stateOfInterest){
        case(SWQ):
            _stateHistoryStruct.p_SWQ = _stateHistoryStruct.n_SWQ/get_totalStateChanges();
        case(UWQ):
            _stateHistoryStruct.p_UWQ = _stateHistoryStruct.n_UWQ/get_totalStateChanges();
        case(FWQ):
            _stateHistoryStruct.p_FWQ = _stateHistoryStruct.n_FWQ/get_totalStateChanges();
    }
}
uint8_t rmsClass::get_stateHistoryPercentage(RMSState stateOfInterest){
    switch(stateOfInterest){
        case(SWQ):
            return _stateHistoryStruct.p_SWQ; 
        case(UWQ):
            return _stateHistoryStruct.p_UWQ;
        case(FWQ):
            return _stateHistoryStruct.p_FWQ;
        default:
            Serial.print("Not a valid state");
            return 2;
    }
}

void rmsClass::reset_History(){
    set_stateHistoryCount(SWQ, 0);
    set_stateHistoryCount(UWQ, 0);
    set_stateHistoryCount(FWQ, 0);

    updateTotalStateChanges(); //redundant, because this is done in the other function

    set_stateHistoryPercentage(SWQ);
    set_stateHistoryPercentage(UWQ);
    set_stateHistoryPercentage(FWQ);
}

void rmsClass::update_wmAlarmSituation(uint32_t new_currentAlarmEPochTime){
    set_wmCurrentAlarmEPochTime(new_currentAlarmEPochTime);

    Serial.print("get_wmCurrentAlarmEPochTime(): ");
    Serial.println(get_wmCurrentAlarmEPochTime());
    Serial.print("get_wmLastAlarmEPochTime(): ");
    Serial.println(get_wmLastAlarmSMSEPochTime());
    Serial.print("Time difference: ");
    Serial.println(get_wmCurrentAlarmEPochTime()-get_wmLastAlarmSMSEPochTime());
    Serial.print("Allowed interval: ");
    Serial.println(get_wmAllowedIntervalBetweenSMS());
    Serial.print("second condition: ");
    bool test = (get_wmCurrentAlarmEPochTime()-get_wmLastAlarmSMSEPochTime() > get_wmAllowedIntervalBetweenSMS());
    Serial.println (test);

    if ((get_wmAlarmSituation() == 0) && (get_rmsState() == UWQ || get_rmsState() == FWQ)){
        set_wmLastAlarmSMSEPochTime(get_wmCurrentAlarmEPochTime());

        set_wmAlarmSituation(1);
    }
    else if ((get_wmAlarmSituation() == 2) && (get_wmCurrentAlarmEPochTime()-get_wmLastAlarmSMSEPochTime() > get_wmAllowedIntervalBetweenSMS())){
        set_wmAlarmSituation(2);
        set_wmLastAlarmSMSEPochTime(get_wmCurrentAlarmEPochTime());

        if (get_rmsState() == SWQ){ // THIS is where we can tune the settings to stop the alarm sending
            set_wmAlarmSituation(3);
        }
    }
}

void rmsClass::set_wmAlarmSituation(uint8_t new_wmAlarmSituation){
    _wmStruct.alarmSituation = new_wmAlarmSituation;
}

uint8_t rmsClass::get_wmAlarmSituation(){
    return _wmStruct.alarmSituation;
}
// void rmsClass::set_inHistoryWindow(){
//     RMSState currentState = get_rmsState();
//     switch(currentState){
//         case(SWQ):
//             break;
//         case(UWQ):
//             _wmStruct.inSendingHistoryWindow = true;
//             break;
//         case(FWQ):
//             _wmStruct.inSendingHistoryWindow = true;
//             break;
//         default:
//             Serial.print("No state defined, no trigger");
//     }
// }



