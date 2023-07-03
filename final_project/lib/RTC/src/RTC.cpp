#include <Arduino.h>
#include <TimeLib.h>

#include "Constant.h"
#include "Global.h"

#include "RTC.h"

#include "States.h"

#include "Tool.h"


// uint32_t currentEpochTime = ;

bool RTC_init(void){
  bool success = true;
  rtc.begin();
  rtcDS3231.begin();

  RTC_updateInternalRTCToCurrentTime();

  rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
  rtc.attachInterrupt(RTC_callbackAlarmMatch);
  return success;
}

void RTC_updateInternalRTCToCurrentTime(void){
  //assuming the rtcDS3231 had already been initialised
  DateTime now = rtcDS3231.now();

  //using external RTC OPTION
  rtc.setEpoch(now.unixtime()); 
}

void RTC_callbackAlarmMatch(){
  // //when match alarm, inform an alarm match occured
  Tool_setBitOn(&triggeredInputEvent, WM_INPUTBIT);
  // glpbal variable: hecne update the time at which alarm occured
  alarmMatchEPochTime = rtc.getEpoch();
  // in case alarm match is after next hb epoch time,
  if (alarmMatchEPochTime >= hbEPochTime){
    // inform RMS that it will need to perform a HB signla
    Tool_setBitOn(&triggeredInputEvent, HB_INPUTBIT);
  }
}

bool RTC_setUpHB(ConfigurationStruct cfgStructArg){
  bool success = true; 
  uint32_t currentTime = rtc.getEpoch();
  hbEPochTime = RTC_findNextHBEPochTime(currentTime, cfgStructArg);
  if (hbEPochTime != 0 
  && hbEPochTime > currentTime){
    success = true;
  }
  else{
    success = false;
  }
  return success;
}

//TODO: add configuration struct to set the appropriate time
uint32_t RTC_findNextHBEPochTime(uint32_t currentEPochTime, ConfigurationStruct cfgStructArg){

  // Convert the given epoch time to a tm structure
  tmElements_t targetTime;
  breakTime(currentEPochTime, targetTime);

  // Increment the date by one day if the current time is after 9 AM
  // CONFIG: set the time at which the first HB should occur
  if (targetTime.Hour >= cfgStructArg.hbTargetHour) {
    currentEPochTime += cfgStructArg.hbIntervalHour * 60 * 60; // Add 24 hours in seconds
  }

  // break down the updated (by 24hours) currentEpochTime
  breakTime(currentEPochTime, targetTime);

  // Set the time to 9AM on the targetTime
  targetTime.Hour = cfgStructArg.hbTargetHour;
  targetTime.Minute = 0;
  targetTime.Second = 0;

  //Convert the resulting time to epOchTime
  uint32_t nextHBEPochtTime = makeTime(targetTime);
  Serial.println("Next HB EPochTime: ");
  Serial.println(nextHBEPochtTime);
  char buf[40];
  Tool_stringTime(nextHBEPochtTime, buf);
  Serial.println(buf);
  return nextHBEPochtTime;
}

//TODO: include COnfiguration struct
uint32_t RTC_updateHBEPochTime(uint32_t hbEPochTime, ConfigurationStruct cfgStructArg){
  // CONFIG: tune how often we want a HB
  return hbEPochTime+cfgStructArg.hbIntervalHour*60*60;
}

// /* convert the time stamp into readable form */
// void RTC_getTimeInText(uint32_t ePochTime, char* buf){
//   // Convert the timestamp to a tm structure
//   tmElements_t timeInfo;
//   breakTime(ePochTime, timeInfo);

//   // Format the time components into the provided buffer
//   sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", timeInfo.Year + 1970, timeInfo.Month, timeInfo.Day, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);
//   // sprintf(buf, "%4d%02d%02d%02d%02d%02d", timeInfo.Year + 1970, timeInfo.Month, timeInfo.Day, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);
// }




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