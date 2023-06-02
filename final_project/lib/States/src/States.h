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

      float get_orpReading();
      void set_orpReading(float new_orpReading);

      uint32_t get_sleepPeriod();
      void set_sleepPeriod();

      uint8_t get_sleepPeriod(RMSState anyState);


      struct alarmStruct{
         uint32_t lastAlarmSMSEPochTime;
         uint32_t currentAlarmEPochTime; 
         uint32_t allowedIntervalBetweenSMS;

         //constructor
         alarmStruct(uint32_t lastAlarmSMSEPochTime_initVal, 
                     uint32_t currentAlarmEPochTime_initVal, 
                     uint32_t allowedIntervalBetweenSMS_initVal);

      };

      //URA
      void set_URAlastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_URAlastAlarmSMSEPochTime();

      void set_URAcurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime);
      uint32_t get_URAcurrentAlarmEPochTime();

      uint32_t get_URAallowedIntervalBetweenSMS();

      bool ura_canSendSMS(uint32_t new_currentAlarmEPochTime);

      //WM
      void set_wmLastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_wmLastAlarmSMSEPochTime();

      void set_wmCurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime);
      uint32_t get_wmCurrentAlarmEPochTime();

      uint32_t get_wmAllowedIntervalBetweenSMS();

      bool wm_canSendSMS(uint32_t new_currentAlarmEPochTime);


      // file management
      /*add a file cursor to know where was the further most away timestamp*/
      
      struct measHistoryStruct {
         uint8_t n_SWQ;
         uint8_t n_UWQ;
         uint8_t n_FWQ;

      };

      


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

      float _orpReading = 0;

      /* -1 is to account for the delay of 1 sec after the occurence of the alram match. 
      This is because the alarm 0 interrupt flag in the interrupt flag status and clear registers 
      is set on the next 0-1 transition of CLK_RTC_CNT.
      */
      static const uint8_t _SWQSleepPeriod = 10-1; //sec
      static const uint8_t _UWQSleepPeriod = 10-1; //sec
      static const uint8_t _FWQSleepPeriod = 10-1; //sec

      alarmStruct _uraStruct;
      alarmStruct _wmStruct;

      void set_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg, uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg);

      void set_currentAlarmEPochTime(alarmStruct& alarmStructArg, uint32_t new_currentAlarmEPochTime);
      uint32_t get_currentAlarmEPochTime(alarmStruct& alarmStructArg);

      uint32_t get_allowedIntervalBetweenSMS(alarmStruct& alarmStructArg);

      



};

#endif