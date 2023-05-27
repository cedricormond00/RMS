//FSM.h
// functions related to FSM 

#ifndef FSM_H
#define FSM_H

#include <Arduino.h>
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib

#include "States.h"

void FSM_updateInputEventCode(uint8_t *eventInputCode, volatile uint8_t* triggeredInputEvent);//input arguments: relevant function counter
// void FSM_executeFunction(uint8_t* eventInputCode_, char ORPData_[]);
void FSM_waterMonitoring(char ORPData_[]); // not need for any pointer dereferinc on an array. This is done by default

// void FSM_executeFunction(uint8_t* eventInputCode_, Ezo_board* EZO_ORP, RMSState* currentState);
// void FSM_f_WaterMonitoring_EZO(Ezo_board* classArg, RMSState* currentState);
void FSM_executeFunction(uint8_t* eventInputCode_, Ezo_board* EZO_ORP, rmsClass& rmsClassArg, RTCZero& rtcClassArg, RMSState* currentState);
void FSM_f_WaterMonitoring_EZO(Ezo_board* classArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg, RMSState* currentState);

void FSM_getEzoWaterReading(Ezo_board* classArg);

void FSM_f_URA();

#endif