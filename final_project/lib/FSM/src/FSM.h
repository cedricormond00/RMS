//FSM.h
// functions related to FSM 

#ifndef FSM_H
#define FSM_H

#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib


void FSM_updateEventInputCode(uint8_t *eventInputCode, uint16_t waterMonitoringPeriod);//input arguments: relevant function counter
// void FSM_executeFunction(uint8_t* eventInputCode_, char ORPData_[]);
void FSM_waterMonitoring(char ORPData_[]); // not need for any pointer dereferinc on an array. This is done by default

void FSM_executeFunction(uint8_t* eventInputCode_, char ORPData_[], Ezo_board* EZO_ORP);
void FSM_waterMonitoring_EZO(Ezo_board* classArg);

#endif