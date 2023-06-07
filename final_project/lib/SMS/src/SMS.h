//SMS.h
#ifndef SMS_H
#define SMS_H

#include "States.h"

void SMS_uraSend(rmsClass& rmsClassArg);
void SMS_wmSend(rmsClass& rmsClassArg);
void SMS_hbSend(rmsClass& rmsClassArg);

void SMS_wmImmediate(rmsClass& rmsClassArg);
void SMS_wmHistoryWindow(rmsClass& rmsClassArg);

#endif