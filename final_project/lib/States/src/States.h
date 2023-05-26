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

/* include a class that contains 
- the time since at wakeup
-  or a way to track the time between monitoring

djd
*/ 
class rmsClass {
   public:
      rmsClass();
      
      RMSState get_rmsState();
      void set_rmsState(RMSState newState);

      uint32_t get_wakeUpEPochTime();
      void set_wakeUpEPochTime(uint32_t new_wakeUpEPochTime);

      uint32_t get_toSleepEPochTime();
      void set_toSleepEPochTime(uint32_t new_toSleepEPochTime);

   private:
      // enum RMSState {
      //    INIT = 0,
      //    SWQ,
      //    UWQ,
      //    FWQ,
      //    SLEEP
      // };
      enum RMSState _rmsState;
      uint32_t _wakeUpEPochTime;  // epoch time at wakup
      uint32_t _toSleepEPochTime; // epoch time when going to sleep



};

#endif