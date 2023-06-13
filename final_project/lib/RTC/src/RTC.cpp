#include <Arduino.h>
#include <TimeLib.h>

#include "Constant.h"
#include "Global.h"

#include "RTC.h"

#include "States.h"

#include "Tool.h"


// uint32_t currentEpochTime = ;


void RTC_init(void){

  rtc.begin();
  rtcDS3231.begin();

  //assuming the rtcDS3231 had already been initialised
  DateTime now = rtcDS3231.now();

  //using external RTC OPTION
  rtc.setEpoch(now.unixtime()); 

  //MANUAL OPTION
  // rtc.setEpoch(1577836800); // Jan 1, 2020
  // rtc.setDate(01, 01, 2020);
  // rtc.setTime(0, 0, 0);
  //SERIAL OPTION
  // while (Serial.available()) {
  //     Serial.read();  // Clear any existing data from the serial buffer
  // }
  // Serial.println("GET_EPOCH");  // Send a command to the computer to request the epoch time
  // while (!Serial.available()) {
  //     ;  // Wait for the response from the computer
  // }
  // uint32_t epochTime = Serial.parseFloat();  // Read the epoch time from the serial connection
  // rtc.setEpoch(epochTime);  // Set the obtained epoch time
  // PREPROCESSING OPTION
  // rtc.setEpoch(currentEpochTime);

  rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
  rtc.attachInterrupt(RTC_callbackAlarmMatch);

  //configure next 9AM HB alarm time
  hbEPochTime = RTC_findNext9AMEPochTime(now.unixtime());

}

void RTC_updateInternalRTCToCurrentTime(void){
  //assuming the rtcDS3231 had already been initialised
  DateTime now = rtcDS3231.now();

  //using external RTC OPTION
  rtc.setEpoch(now.unixtime()); 
}

void RTC_callbackAlarmMatch(){
  // //when match alarm, set all other bits to zero
  // Tool_setBitOff(&triggeredInputEvent, ~WM_INPUTBIT);
  Tool_setBitOn(&triggeredInputEvent, WM_INPUTBIT);
  alarmMatchEPochTime = rtc.getEpoch();
  if (alarmMatchEPochTime >= hbEPochTime){
    Tool_setBitOn(&triggeredInputEvent, HB_INPUTBIT);
  }
}

uint32_t RTC_findNext9AMEPochTime(uint32_t currentEPochTime){
  // Convert the given epoch time to a tm structure
  tmElements_t targetTime;
  breakTime(currentEPochTime, targetTime);

  // Increment the date by one day if the current time is after 9 AM
  // CONFIG: set the time at which the first HB should occur
  if (targetTime.Hour >= 17) {
    currentEPochTime += 24 * 60 * 60; // Add 24 hours in seconds
  }

  // break down the updated (by 24hours) currentEpochTime
  breakTime(currentEPochTime, targetTime);

  // Set the time to 9AM on the targetTime
  targetTime.Hour = 17;
  targetTime.Minute = 0;
  targetTime.Second = 0;

  //Convert the resulting time to epOchTime
  uint32_t next9AMEPochtTime = makeTime(targetTime);
  return next9AMEPochtTime;
}

uint32_t RTC_updateHBEPochTime(uint32_t hbEPochTime){
  // CONFIG: tune how often we want a HB
  return hbEPochTime+24*60*60;
}

/* convert the time stamp into readable form */
void RTC_getTimeInText(uint32_t ePochTime, char* buf){
  // Convert the timestamp to a tm structure
  tmElements_t timeInfo;
  breakTime(ePochTime, timeInfo);

  // Format the time components into the provided buffer
  sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", timeInfo.Year + 1970, timeInfo.Month, timeInfo.Day, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);
  // sprintf(buf, "%4d%02d%02d%02d%02d%02d", timeInfo.Year + 1970, timeInfo.Month, timeInfo.Day, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);
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
  Tools_print2digits(rtcClassArg.getHours());
  Serial.print(":");
  Tools_print2digits(rtcClassArg.getMinutes());
  Serial.print(":");
  Tools_print2digits(rtcClassArg.getSeconds());

  Serial.println();
}