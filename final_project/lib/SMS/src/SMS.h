//SMS.h
#ifndef SMS_H
#define SMS_H

#include "States.h"
#include "Configuration.h"

/**
 * @brief function to initialisze nb object 
 * @details sets 
 * - the timeout 
*/
void SMS_init(uint32_t timeout);

void SMS_wmSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);
void SMS_wmImmediate(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);
void SMS_wmHistoryWindow(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

void SMS_uraSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

void SMS_hbSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

void SMS_BUPSendEnergyLevel(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);
void SMS_BUPSendIsStablePowerSupply(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);


bool SMS_initConnection();
// bool SMS_sendMessage(char message[160], bool sendSMS, char remoteNumber[20]);
bool SMS_sendMessage(char message[160], char remoteNumber[20]);
void SMS_sendSMS(ConfigurationStruct cfgStructArg, char message[160]);
#endif