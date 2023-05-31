// Tool.h
#ifndef STATES_H // include guard
#define STATES_H

#include <Arduino.h>

#include <RTCZero.h>


// types
enum RMSState {
   INIT = 0,
   SWQ,
   UWQ,
   FWQ,
   SLEEP
};

/* Class that contains 
- current RMS State
- previous RMS State
- inputEvent Code
- 

djd
*/ 
class rmsClass {
   public:
      rmsClass();
      
      RMSState get_rmsState();
      void set_rmsState(RMSState newState);

      RMSState get_previousRMSState();

      uint8_t get_inputEventCode();
      void set_inputEventCode(uint8_t new_inputEventCode);

      uint32_t get_wmWakeUpEPochTime();
      void set_wmWakeUpEPochTime(uint32_t new_wmWakeUpEPochTime);

      uint32_t get_wmReadEPochTime();
      void set_wmReadEPochTime(uint32_t new_wmReadEPochTime);
      
      uint32_t get_wakeUpEPochTime();
      void set_wakeUpEPochTime(uint32_t new_WakeUpEPochTime);

      uint32_t get_nextWakeUpEPochTime();
      void set_nextWakeUpEPochTime(uint32_t new_nextWakeUpEPochTime);

      uint32_t get_toSleepEPochTime();
      void set_toSleepEPochTime(uint32_t new_toSleepEPochTime);

      uint32_t get_sleepPeriod();
      void set_sleepPeriod();

      uint8_t get_sleepPeriod(RMSState anyState);

   private:
      // enum RMSState {
      //    INIT = 0,
      //    SWQ,
      //    UWQ,
      //    FWQ,
      //    SLEEP
      // };
      enum RMSState _rmsState;
      enum RMSState _previousRMSState;

      uint8_t _inputEventCode;

      uint32_t _wmReadEPochTime;
      uint32_t _wmWakeUpEPochTime;

      uint32_t _nextWakeUpEPochTime; //epoch time when the device will wakeup

      uint32_t _wakeUpEPochTime;  // epoch time at wakup
      uint32_t _toSleepEPochTime; // epoch time when going to sleep
      uint32_t _sleepPeriod; // how long the device should go to sleep for (in sec)

      /* -1 is to account for the delay of 1 sec after the occurence of the alram match. 
      This is because the alarm 0 interrupt flag in the interrupt flag status and clear registers 
      is set on the next 0-1 transition of CLK_RTC_CNT.
      */
      static const uint8_t _SWQSleepPeriod = 10-1; //sec
      static const uint8_t _UWQSleepPeriod = 10-1; //sec
      static const uint8_t _FWQSleepPeriod = 10-1; //sec

      



};

#endif