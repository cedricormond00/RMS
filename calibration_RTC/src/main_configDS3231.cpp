// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include "MKRNB.h"


RTC_DS3231 rtcDS3231;
NB nbAccess;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/** Indicate what RTC calibration you want to use:
 * 1: using compile time
 * 2: manually setting time
 * 3: using network time
 */

uint8_t calMode = 1;

//change to fit, if calMode == 2
uint16_t year = 2023;
uint8_t month = 1;
uint8_t day = 1;
uint8_t hour = 1;
uint8_t minutes = 0;
uint8_t seconds = 0;

void setup () {
  Serial.begin(9600);
  Wire.begin();


  if (! rtcDS3231.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtcDS3231.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtcDS3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }


  uint32_t ePochTime;
  DateTime dt;

  switch (calMode)
  {
  case (1):
    rtcDS3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
    break;
  case (2):
    rtcDS3231.adjust(DateTime(year, month, day, hour, minutes, seconds));
    break;
  case (3):
    ePochTime = nbAccess.getTime();
    dt = DateTime(ePochTime);
    rtcDS3231.adjust(DateTime(dt.year(), dt.month(), dt.day(), dt.minute(), dt.second()));
    break;
  default:
    break;
  }
}

void loop () {
    DateTime now = rtcDS3231.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();

    Serial.print("Temperature: ");
    Serial.print(rtcDS3231.getTemperature());
    Serial.println(" C");

    Serial.println();
    delay(3000);
}
