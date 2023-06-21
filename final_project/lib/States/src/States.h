// Tool.h
#ifndef STATES_H // include guard
#define STATES_H

#include <Arduino.h>

#include <RTCZero.h>

// #include "Configuration.h"


// types
enum RMSState {
   INIT = 0,
   SWQ,
   UWQ,
   FWQ,
   SLEEP,
   BATTERY_LOW,
   BATTERY_NOTCONNECTED,
   SDCARD_NOK,
   SDCARD_FILENAMENOK,
   RTC_FAILEDINIT,
   RTC_FAILEDHBSET
   // POWERSUPPLY_NOTSTABLE,
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

      //TODO: if I chopse to not use these values, but directly access the data from the configuration file, I can delete these functions and variables
      void set_SWQSleepPeriod(uint8_t new_SWQSleepPeriod);
      void set_UWQSleepPeriod(uint8_t new_UWQSleepPeriod);
      void set_FWQSleepPeriod(uint8_t new_FWQSleepPeriod);

      // getter and setter function of rms

      RMSState get_rmsState();
      void set_rmsState(RMSState newState);

      // RMSState get_previousRMSState();

      uint8_t get_inputEventCode();
      void set_inputEventCode(uint8_t new_inputEventCode);

      uint32_t get_wmWakeUpEPochTime();
      void set_wmWakeUpEPochTime(uint32_t new_wmWakeUpEPochTime);

      uint32_t get_wmReadEPochTime();
      void set_wmReadEPochTime(uint32_t new_wmReadEPochTime);
      
      uint32_t get_wakeUpEPochTime();
      void set_wakeUpEPochTime(uint32_t new_WakeUpEPochTime);

      //defined during the WM routine, since the next wakeup time depends on the RMSState
      uint32_t get_nextWakeUpEPochTime();
      void set_nextWakeUpEPochTime(uint32_t new_nextWakeUpEPochTime);

      uint32_t get_toSleepEPochTime();
      void set_toSleepEPochTime(uint32_t new_toSleepEPochTime);

      float get_orpReading();
      void set_orpReading(float new_orpReading);

      uint32_t get_sleepPeriod();
      void set_sleepPeriod();

      //TODO: set sleep period from SD card in a setup routine to set the rms machine to a known state
      uint8_t get_sleepPeriod(RMSState anyState);

      
      enum SMSState{
         NOANOMALIES = 0,
         FIRSTANOMALY,
         HWANOMALIES,
         NORMALOCCURENCE
      };
      
      /** \brief Struct containing the information regarding the SMS sent from a certain trigger
       * \remarks Use cases include URA or WM SMS multialarm management
      */
      struct alarmStruct{
         uint32_t lastAlarmSMSEPochTime;
         uint32_t currentAlarmEPochTime; 
         uint32_t allowedIntervalBetweenSMS;
         bool inSendingHistoryWindow = true; //informs whether we are in a continuous sending window from the WaterMontioring
         SMSState alarmSituation = NOANOMALIES;   /* 0 = first UWQ / FWQ -> go straight to sending SMS
                                          1 = collate samples over HW. -> once the HW time has elapsed, go to send an SMS: If at that time, the last reading is an SWQ, go to send final SMS 
                                          2 = if the final reading was SWQ (from HW collating) -> rthen set alarmsituation to 0 */
         //constructor
         //TODO: isntead of using a constructor: set these up in the initialisation routine of rms
         alarmStruct(uint32_t lastAlarmSMSEPochTime_initVal, 
                     uint32_t currentAlarmEPochTime_initVal, 
                     uint32_t allowedIntervalBetweenSMS_initVal);

      };
      

      //URA
      void set_URAlastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_URAlastAlarmSMSEPochTime();

      void set_URAcurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime);
      uint32_t get_URAcurrentAlarmEPochTime();

      //TODO: instead of using this,. directly use the value from the cfg file
      void set_URAallowedIntervalBetweenSMS(uint32_t new_allowedIntervalBetweenSMS);
      uint32_t get_URAallowedIntervalBetweenSMS();

      bool ura_canSendSMS(uint32_t new_currentAlarmEPochTime);

      //WM
      void set_wmLastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_wmLastAlarmSMSEPochTime();

      void set_wmCurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime);
      uint32_t get_wmCurrentAlarmEPochTime();

      void set_wmAllowedIntervalBetweenSMS(uint32_t new_allowedIntervalBetweenSMS);
      uint32_t get_wmAllowedIntervalBetweenSMS();

      bool wm_canSendSMS(uint32_t new_currentAlarmEPochTime);

      void update_wmAlarmSituation(uint32_t new_currentAlarmEPochTime);
      void set_wmAlarmSituation(SMSState new_wmAlarmSituation);
      SMSState get_wmAlarmSituation();

      // file management
      /*add a file cursor to know where was the further most away timestamp*/
      
      struct stateHistoryStruct {
         uint8_t n_SWQ;
         uint8_t n_UWQ;
         uint8_t n_FWQ;
         uint8_t n_meas;
         float p_SWQ;
         float p_UWQ;
         float p_FWQ;
      };

      void set_stateHistoryCount(RMSState stateOfInterest, uint8_t countState);
      uint8_t get_stateHistoryCount(RMSState stateOfInterest);

      uint8_t get_totalStateChanges();

      void set_stateHistoryPercentage(RMSState stateOfInterest);
      float get_stateHistoryPercentage(RMSState stateOfInterest);

      void reset_History();

      // Battery 
      enum BatteryEnergyLevelState{
         criticalEL,
         lowEL,
         sufficientEL
      };
      /** \brief struct to contain state of the battery
       * \remarks this will be updated each time a WM or URA event occurs
      */
      struct powerStruct {
         float batteryVoltage;
         bool isStablePowerSupply;
         uint8_t chargeStatus;
         BatteryEnergyLevelState batteryELState;
      };
      void set_powerStructBatteryVoltage(float new_batteryVoltage);
      float get_powerStructBatteryVoltage();

      BatteryEnergyLevelState get_powerStructBatteryELState();

      void set_powerStructStablePowerSupply(bool new_isStablePowerSupply);
      bool get_powerStructStablePowerSupply();

      void set_powerStructChargeStatus(uint8_t new_chargeStatus);
      uint8_t get_powerStructChargeStatus();
      
      /** \brief struct to contain information about sms sent about the battery
       * \remarks this will be updated each time a WM or URA event occurs
      */
      struct smsPowerStruct{
         // variables to contain the status of the last sent SMS
         BatteryEnergyLevelState batteryEnergyLevelState;
         bool isStablePowerSupply;

         uint32_t energyLevelStateSMSSentEPochTime;
         uint32_t isStablePowerSupplySMSSentEPochTime;
      };
      /*Initailasisze the sms status for power struct*/
      void init_smsPowerStruct(uint32_t ePochTime);

      void set_smsPowerStructBatteryEnergyLevelState(BatteryEnergyLevelState new_batteryELState, uint32_t ePochTime);
      BatteryEnergyLevelState get_smsPowerStructBatteryEnergyLevelState();

      void set_smsPowerStructIsStablePowerSupply(bool new_isStablePowerSupply, uint32_t ePochTime);
      bool get_smsPowerStructIsStablePowerSupply();

      uint32_t get_smsPowerStructEnergyLevelSMSSentEPochTime();
      uint32_t get_smsPowerStructIsStablePowerSupplySMSSentEPochTime();



      // void set_powerStructMember(uint8_t memberIndex, float new_batteryVoltage);
      // uint8_t get_powerStructMember(uint8_t memberIndex);



      


   private:
      // enum RMSState {
      //    INIT = 0,
      //    SWQ,
      //    UWQ,
      //    FWQ,
      //    SLEEP
      // };
      enum RMSState _rmsState;
      // enum RMSState _previousRMSState;

      uint8_t _inputEventCode;

      //useful for datalogging
      uint32_t _wmReadEPochTime;
      // useful for computing the right time to wakeup
      uint32_t _wmWakeUpEPochTime;
      // time when the device will next wakeup
      uint32_t _nextWakeUpEPochTime; 

      // TODO: (DEL) I am not sure of the value of this variable. We could potentiially remove it. It may be useful for some checks
      uint32_t _wakeUpEPochTime;  // epoch time at wakup
      uint32_t _toSleepEPochTime; // epoch time when going to sleep
      /* how long the device should go to sleep for (in sec)
      this value depends on the rmsState
      */
      uint32_t _sleepPeriod; 

      float _orpReading = 0;

      /* -1 is to account for the delay of 1 sec after the occurence of the alram match. 
      This is because the alarm 0 interrupt flag in the interrupt flag status and clear registers 
      is set on the next 0-1 transition of CLK_RTC_CNT.
      */
      uint8_t _SWQSleepPeriod;// = 10-1; //sec
      uint8_t _UWQSleepPeriod;// = 10-1; //sec
      uint8_t _FWQSleepPeriod;// = 10-1; //sec

      alarmStruct _uraStruct;
      alarmStruct _wmStruct;

      void set_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg, uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg);

      void set_currentAlarmEPochTime(alarmStruct& alarmStructArg, uint32_t new_currentAlarmEPochTime);
      uint32_t get_currentAlarmEPochTime(alarmStruct& alarmStructArg);

      void set_allowedIntervalBetweenSMS(alarmStruct& alarmStructArg, uint32_t new_allowedIntervalBetweenSMS);
      uint32_t get_allowedIntervalBetweenSMS(alarmStruct& alarmStructArg);

      stateHistoryStruct _stateHistoryStruct;

      uint8_t updateTotalStateChanges();

      powerStruct _powerStruct;

      smsPowerStruct _smsPowerStruct;

};

#endif