// Tool.h
#ifndef STATES_H // include guard
#define STATES_H

#include <Arduino.h>

#include <RTCZero.h>

// #include "Configuration.h"


// types
/**
 * @brief Enum defining the different states of the RMS.
 */
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
   SMS_NBNOK,
   RTC_FAILEDINIT,
   RTC_FAILEDHBSET, 
   UNKNOWN_ERROR
   // POWERSUPPLY_NOTSTABLE,
};

/**
 * @brief Class that contains the RMS state and related information.
 */
class rmsClass {
   public:
      /**
      * @brief Constructor for the rmsClass.
      */
      rmsClass();

      // RMS State

      //TODO: if I chopse to not use these values, but directly access the data from the configuration file, I can delete these functions and variables
      /**
       * @brief Setter function for the SWQ sleep period.
       * @param new_SWQSleepPeriod The new SWQ sleep period value.
       * @return None.
       */
      void set_SWQSleepPeriod(uint16_t new_SWQSleepPeriod);
      /**
       * @brief Setter function for the UWQ sleep period.
       * @param new_UWQSleepPeriod The new UWQ sleep period value.
       * @return None.
       */
      void set_UWQSleepPeriod(uint16_t new_UWQSleepPeriod);
      /**
       * @brief Setter function for the FWQ sleep period.
       * @param new_FWQSleepPeriod The new FWQ sleep period value.
       * @return None.
       */
      void set_FWQSleepPeriod(uint16_t new_FWQSleepPeriod);

      /**
       * @brief Getter function for the current RMS state.
       * @param None.
       * @return _rmsState The current RMS state.
       */
      RMSState get_rmsState();
      /**
       * @brief Setter function for the RMS state.
       * @param newState The new RMS state.
       * @return None.
       */
      void set_rmsState(RMSState newState);

      /**
       * @brief Getter function for the input event code
       * @param none 
       * @return _inputEventCode.
       * @details This is useful to know what function must be performed
       */
      uint8_t get_inputEventCode();
      /**
       * @brief Setter function for the input event code.
       * @param new_inputEventCode The new input event code.
       * @return None.
       */
      void set_inputEventCode(uint8_t new_inputEventCode);


      /**
       * @brief Getter function for the wakeUpEPochTime.
       * @param None.
       * @return The wakeUpEPochTime.
       * @details this refers to the time at which the device has woken up. Regardless of the event causing the wakeup
       */
      uint32_t get_wakeUpEPochTime();
      /**
       * @brief Setter function for the wakeUpEPochTime.
       * @param new_wakeUpEPochTime The new wakeUpEPochTime.
       * @return None.
       * @details this refers to the time at which the device has woken up
       */
      void set_wakeUpEPochTime(uint32_t new_WakeUpEPochTime);


      /**
       * @brief Getter function for the wmReadEPochTime.
       * @param None.
       * @return The wmReadEPochTime.
       * @details this refers to the time at which an alarm match occured. 
       * This is updated only upon WM function execution
       */
      uint32_t get_wmWakeUpEPochTime();
      /**
       * @brief Setter function for the wmReadEPochTime.
       * @param new_wmReadEPochTime The new wmReadEPochTime.
       * @return None.
       * @details this refers to the time at which an alarm match occured. 
       * This is updated only upon WM function execution
       */
      void set_wmWakeUpEPochTime(uint32_t new_wmWakeUpEPochTime);

      /**
       * @brief Getter function for the wmWakeUpEPochTime.
       * @param None.
       * @return The wmWakeUpEPochTime.
       * @details this time refers to any time a water sampling is performed, regardless of the event causing it (URA or WM), confusingly
       */
      uint32_t get_wmReadEPochTime();
      /**
       * @brief Setter function for the wmWakeUpEPochTime.
       * @param new_wmWakeUpEPochTime The new wmWakeUpEPochTime.
       * @return None.
       */
      void set_wmReadEPochTime(uint32_t new_wmReadEPochTime);
      


      /**
       * @brief Getter function for the nextWakeUpEPochTime.
       * @param None.
       * @return The nextWakeUpEPochTime.
       * @details This refers tot the time at which the device will need to wakeup
       * This value is computed during the WM (this time function only, not from an URA) routine, since the next wakeup time depends on the RMSState
       */
      uint32_t get_nextWakeUpEPochTime();
      /**
       * @brief Setter function for the nextWakeUpEPochTime.
       * @param new_nextWakeUpEPochTime The new nextWakeUpEPochTime.
       * @return None.
       * @details This refers tot the time at which the device will need to wakeup
       * This value is computed during the WM (this time function only, not from an URA) routine, since the next wakeup time depends on the RMSState
       */
      void set_nextWakeUpEPochTime(uint32_t new_nextWakeUpEPochTime);


      uint32_t get_toSleepEPochTime();
      void set_toSleepEPochTime(uint32_t new_toSleepEPochTime);

      float get_orpReading();
      void set_orpReading(float new_orpReading);

      uint32_t get_sleepPeriod();
      void set_sleepPeriod();

      uint8_t get_sleepPeriod(RMSState anyState);


      

      // SMS Alarm raising

      /**
       * @brief states for the SMS sending: 
       * @details used to keep track of which SMS has been sent yet, during the water moiniotring function (WM)
       */      
      enum SMSState{
         NOANOMALIES = 0,
         FIRSTANOMALY,
         HWANOMALIES,
         NORMALOCCURENCE
      };
      
      /** 
       * @brief Struct containing the information regarding the SMS sent from a certain trigger
       * @remarks Use cases include URA or WM SMS multialarm management
      */
      struct alarmStruct{
         // time at which the last SMS was sent
         uint32_t lastAlarmSMSEPochTime;
         // time at which the current alarm match occured: larger or equalt to  lastAlarmSMSEPochTime
         uint32_t currentAlarmEPochTime; 
         // how long in between each SMS is permitted
         // UPGRADE: this is redundant: could either jsut us ethe info present in the confi file, or the private variables
         uint32_t allowedIntervalBetweenSMS; // in seconds
         /* 
         informs whether we are in a history window.
         If we are, we may not send another SMS
         If we are out of it, we may send another SMS
         */
         bool inSendingHistoryWindow = true; 

         SMSState alarmSituation = NOANOMALIES;   

      };


      // URA alarm management function
      void set_URAlastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_URAlastAlarmSMSEPochTime();

      void set_URAcurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime);
      uint32_t get_URAcurrentAlarmEPochTime();

      //UPGRADE:  (TBC) instead of using this,. directly use the value from the cfg file
      void set_URAallowedIntervalBetweenSMS(uint32_t new_allowedIntervalBetweenSMS);
      uint32_t get_URAallowedIntervalBetweenSMS();
     
      /** @brief tells wether a SMS may be sent or not, for the URA situation*/
      bool ura_canSendSMS(uint32_t new_currentAlarmEPochTime);

      // WM alarm management function
      void set_wmLastAlarmSMSEPochTime(uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_wmLastAlarmSMSEPochTime();

      void set_wmCurrentAlarmEPochTime(uint32_t new_currentAlarmEPochTime);
      uint32_t get_wmCurrentAlarmEPochTime();

      void set_wmAllowedIntervalBetweenSMS(uint32_t new_allowedIntervalBetweenSMS);
      uint32_t get_wmAllowedIntervalBetweenSMS();

      /** @brief tells wether a SMS may be sent or not, for the WM situation*/
      bool wm_canSendSMS(uint32_t new_currentAlarmEPochTime);

      // void update_wmAlarmSituation(uint32_t new_currentAlarmEPochTime);
      void set_wmAlarmSituation(SMSState new_wmAlarmSituation);
      SMSState get_wmAlarmSituation();





      // file management
      /*add a file cursor to know where was the further most away timestamp*/
      
      struct stateHistoryStruct {
         bool success;
         uint8_t n_SWQ;
         uint8_t n_UWQ;
         uint8_t n_FWQ;
         uint8_t n_meas;
         float p_SWQ;
         float p_UWQ;
         float p_FWQ;
      };
      void set_stateHistorySuccess(bool new_success);
      bool get_stateHistorySuccess();
      
      /* 
      based on the state, updates:
      - the count fr this state
      - updates the total state changes
      - updates the state History percentage (only for the particular corresponding state)
      TODO: could update all states. COuld also create a function which updates all percentages at once*/
      void set_stateHistoryCount(RMSState stateOfInterest, uint8_t countState);
      uint8_t get_stateHistoryCount(RMSState stateOfInterest);

      uint8_t get_totalStateChanges();

      void set_stateHistoryPercentage(RMSState stateOfInterest);
      float get_stateHistoryPercentage(RMSState stateOfInterest);

      void reset_History();



      // Power Supply Situation 
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

      /**
       * Based on thew battery voltage, updates
       * - the battery volatage,
       * - the ENergy level state
       */
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
      


   private:

      // RMS State
      enum RMSState _rmsState;
      // enum RMSState _previousRMSState;

      uint8_t _inputEventCode;

      /*
      Useful for datalogging
      Beware: this is not only the time at which the watermonitoring from the WM function occured. it may also be used from an URA event: This value holds the time at which a sampling from the water occured
      */
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
      uint16_t _SWQSleepPeriod;// = 10-1; //sec
      uint16_t _UWQSleepPeriod;// = 10-1; //sec
      uint16_t _FWQSleepPeriod;// = 10-1; //sec


      //SMS ALarm raising 

      alarmStruct _uraStruct;
      alarmStruct _wmStruct;

      // Alarm raising utils functions
      void set_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg, uint32_t new_lastAlarmSMSEPochTime);
      uint32_t get_lastAlarmSMSEPochTime(alarmStruct& alarmStructArg);

      void set_currentAlarmEPochTime(alarmStruct& alarmStructArg, uint32_t new_currentAlarmEPochTime);
      uint32_t get_currentAlarmEPochTime(alarmStruct& alarmStructArg);

      void set_allowedIntervalBetweenSMS(alarmStruct& alarmStructArg, uint32_t new_allowedIntervalBetweenSMS);
      uint32_t get_allowedIntervalBetweenSMS(alarmStruct& alarmStructArg);



      // Data/FIle management
      stateHistoryStruct _stateHistoryStruct;

      uint8_t updateTotalStateChanges();


      // Power Supply
      powerStruct _powerStruct;

      smsPowerStruct _smsPowerStruct;

};

#endif