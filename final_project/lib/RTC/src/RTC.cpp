#include "RTC.h"


void RTC_init(RTCZero& rtc){
    rtc.begin();
    rtc.setEpoch(0);
    // rtc.setEpoch(1577836800); // Jan 1, 2020
    // rtc.setDate(01, 01, 2020);
    // rtc.setTime(0, 0, 0);

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

  Serial.print("Seconds since Jan 1 2020 = ");
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

  delay(1000);
}