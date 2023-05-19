//FSM.h
// functions related to FSM 

#ifndef FSM_H
#define FSM_H

void FSM_updateEventInputCode(uint8_t *eventInputCode, uint16_t waterMonitoringPeriod);//input arguments: relevant function counter
void FSM_executeFunction(uint8_t* eventInputCode_, char ORPData_[]);
void FSM_waterMonitoring(char ORPData_[]); // not need for any pointer dereferinc on an array. This is done by default

#endif