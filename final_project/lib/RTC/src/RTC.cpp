#include <TimeLib.h>


#include "RTC.h"
#include "Constant.h"
#include "Global.h"
#include "Tool.h"

#include "States.h"

uint8_t dummyVar = 0;

void RTC_init(RTCZero& rtc){
    rtc.begin();

    rtc.setEpoch(1577836800); // Jan 1, 2020
    // rtc.setDate(01, 01, 2020);
    // rtc.setTime(0, 0, 0);

    // while (Serial.available()) {
    //     Serial.read();  // Clear any existing data from the serial buffer
    // }
    // Serial.println("GET_EPOCH");  // Send a command to the computer to request the epoch time
    // while (!Serial.available()) {
    //     ;  // Wait for the response from the computer
    // }
    // uint32_t epochTime = Serial.parseFloat();  // Read the epoch time from the serial connection
    // rtc.setEpoch(epochTime);  // Set the obtained epoch time

    rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
    rtc.attachInterrupt(RTC_callbackAlarmMatch);

}

// void RTC_callbackAlarmMatch(RMSClass& rmsClassArg, RTCZero& rtcClassArg){
void RTC_callbackAlarmMatch(){
  
  // //when match alarm, set all other bits to zero
  // Tool_setBitOff(&triggeredInputEvent, ~WM_INPUTBIT);
  Tool_setBitOn(&triggeredInputEvent, WM_INPUTBIT);
  alarmMatchEPochTime = rtc.getEpoch();
}


/* convert the time stamp into readable form */
void RTC_getTimeInText(uint32_t ePochTime, char* buf){
  // // Convert the epoch time to time_t
  // time_t time = ePochTime;

  // uint32_t time = ePochTime;

  // Convert the timestamp to a tm structure
  tmElements_t timeInfo;
  breakTime(ePochTime, timeInfo);

  // Format the time components into the provided buffer
  sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", timeInfo.Year + 1970, timeInfo.Month, timeInfo.Day, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);

}


void RTC_print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}


void RTC_printTime(RTCZero& rtcClassArg){
  Serial.print("Unix time = ");
  Serial.println(rtcClassArg.getEpoch());

  Serial.print("Seconds since 01/01/2000 = ");
  Serial.println(rtcClassArg.getY2kEpoch());

  // Print date...
  Serial.print(rtcClassArg.getDay());
  Serial.print("/");
  Serial.print(rtcClassArg.getMonth());
  Serial.print("/");
  Serial.print(rtcClassArg.getYear());
  Serial.print("\t");

  // ...and time
  RTC_print2digits(rtcClassArg.getHours());
  Serial.print(":");
  RTC_print2digits(rtcClassArg.getMinutes());
  Serial.print(":");
  RTC_print2digits(rtcClassArg.getSeconds());

  Serial.println();

}