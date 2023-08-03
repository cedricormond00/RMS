#include <Arduino.h>

#include <RTCZero.h>

#include <ArduinoLowPower.h>

#include "Constant.h"
#include "LED.h"

RTCZero rtc;

// Interrupt handler for RTC alarm match
void rtcAlarmMatch() {
  // Handle the wake-up event here
  ToggleLED(BLUELED_PIN);
}

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}

void setup() {
  while(!Serial);
  // LED pins
  pinMode(BLUELED_PIN, OUTPUT);
  pinMode(REDLED_PIN, OUTPUT);
  pinMode(GREENLED_PIN, OUTPUT);
  pinMode(YELLOWLED_PIN, OUTPUT);
  pinMode(ORANGELED_PIN, OUTPUT);
  digitalWrite(BLUELED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);
  // Initialize the RTC module
  rtc.begin();

  // Set the desired wake-up interval (e.g., 5 seconds)
  rtc.setAlarmSeconds(5);
  rtc.enableAlarm(rtc.MATCH_SS);

  // attach a function to perform upon alarm
  rtc.attachInterrupt(rtcAlarmMatch);

  // Configure the low-power sleep mode
//   LowPower.attachInterruptWakeup(rtcAlarmMatch, WAKEUP_ALARM);

  
}

void loop() {
  Serial.print("Unix time = ");
  Serial.println(rtc.getEpoch()); // Epoch time since 1970

  Serial.print("Seconds since Jan 1 2020 = ");
  Serial.println(rtc.getY2kEpoch()); //since default 200 or set time

  // Print date...
  Serial.print(rtc.getDay()); //uses default or set time
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.print(rtc.getYear());
  Serial.print("\t");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());

  Serial.println();

  // Enter low-power sleep mode
  LowPower.sleep();
}


