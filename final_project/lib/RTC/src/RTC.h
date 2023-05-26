// RTC.h
#ifndef RTC_H // include guard
#define RTC_H

#include <Arduino.h>

#include <RTCZero.h>

void RTC_init(RTCZero& rtc);
void RTC_print2digits(int number);
void RTC_printTime(RTCZero& rtcClassArg);

#endif
