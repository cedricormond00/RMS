#include <Arduino.h>
#include <ArduinoLowPower.h>


#include <iot_cmd.h>
#include <Ezo_i2c_util.h>  
#include <Arduino_PMIC.h>

#include "FSM.h"

// #include "States.h" // normally I should not need this

#include "Constant.h"
#include "Global.h"


#include "Tool.h"

#include "LED.h"
#include "I2c.h"
#include "RTC.h"
#include "Data.h"
#include "EZO.h"
#include "SMS.h"
#include "Battery.h"




bool debugDisplay = 1;


void FSM_updateInputEventCode(rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, volatile uint8_t* triggeredInputEvent)
//TODO: maybe the URA_WAIT bit is useless: just use triggerInput as a means to check 
{

    bool debug=true;
    uint8_t eventInputCode = rmsClassArg.get_inputEventCode();

    // if (debug){
    //     Serial.println("in FSM_updateInputEventCode, function");
    //     Serial.print("eventInputCode before update: ");
    //     Serial.print(*eventInputCode, BIN);
    //     Serial.print(", ");
    //     Serial.println(*eventInputCode);
    //     Serial.print("triggeredInputEvent before update: ");
    //     Serial.print(*triggeredInputEvent, BIN);
    //     Serial.print(", ");
    //     Serial.println(*triggeredInputEvent);
    // }


    if (Tool_isBitOn(*triggeredInputEvent, WM_INPUTBIT)) 
    {
        //if include 9AM alarm -> add the check to see if we are just close to 9AM
        // double gate: one for the time window, and one for a boolean to inform if the HB's been sent. Once out ot the time window, reset to the boolean
        if (debug){
            Serial.println("in FSM_updateInputEventCode, WM");
            Serial.print("eventInputCode before update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
            Serial.print("triggeredInputEvent before update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }
        uint32_t currentTime = rtcClassArg.getEpoch();
        char buf[256];

        RTC_getTimeInText(currentTime, buf);
        
        Serial.print("Current unix time: ");
        Serial.println(currentTime);
        Serial.print("Current unix timestamp: ");
        Serial.println(buf);
        Serial.print("rmsClassArg.get_wakeUpEPochTime(): ");
        Serial.println(rmsClassArg.get_wakeUpEPochTime());
        Serial.print("rmsClassArg.get_wmWakeUpEPochTime(): ");
        Serial.println(rmsClassArg.get_wmWakeUpEPochTime());



        /*
        - Store the time at which the itnernal clock of the RMS matched the alarm
        - This usually occurs when the devie is asleep, and hence wakes the device up
        - Keeping this variable is very important: it allows to know at what time the device will need to wakeup next time
        */
        rmsClassArg.set_wmWakeUpEPochTime(alarmMatchEPochTime);


        Tool_setBitOn(&eventInputCode, WM_INPUTBIT);// code for WM function

        // reset the triggered input event tracker
        Tool_setBitOff(triggeredInputEvent, WM_INPUTBIT);

         if (debug){
            Serial.print("eventInputCode after update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
            Serial.print("triggeredInputEvent after update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }

    }

    if (Tool_isBitOn(*triggeredInputEvent, URA_INPUTBIT)) 
    {
        if (debug && debugDisplay){
            Serial.println("in FSM_updateInputEventCode, URA");
            Serial.print("eventInputCode before update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
            Serial.print("triggeredInputEvent before update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }

        //because we haven't yet reached the 3 seconds
        Tool_setBitOn(&eventInputCode, URA_WAIT_INPUTBIT);// This prevents to go back to sleep
        
        // check the button is still pressed 
        if (digitalRead(BUTTON_PIN)==LOW){
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= 500){
                ToggleLED(YELLOWLED_PIN);
                previousMillis = currentMillis;
            }
            // CONFIG: can change the delay
            if (currentMillis>=(millisOnExternalWakeUp + cfgStructArg.uraPressDuration)){
                digitalWrite(YELLOWLED_PIN, LOW);
                //inform the RMS the URA function may be performed
                Tool_setBitOn(&eventInputCode, URA_INPUTBIT);
                Tool_setBitOff(&eventInputCode, URA_WAIT_INPUTBIT);

                // reset the triggered input event tracker
                Tool_setBitOff(triggeredInputEvent, URA_INPUTBIT);

            }
        }
        else { // the button has been released before the 3 seconds have
            // // no more URA alarm
            Tool_setBitOff(triggeredInputEvent, URA_INPUTBIT);
            // no need to inform that we shall wait for 3 seconds
            Tool_setBitOff(&eventInputCode, URA_WAIT_INPUTBIT);
            digitalWrite(YELLOWLED_PIN, LOW);

        }

        
        if (debug && debugDisplay){
            Serial.print("eventInputCode after update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
            Serial.print("triggeredInputEvent after update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
            debugDisplay = 0;
        }
    }
    if (Tool_isBitOn(*triggeredInputEvent, HB_INPUTBIT)){
        Tool_setBitOn(&eventInputCode, HB_INPUTBIT);// code for WM function

        // reset the triggered input event tracker
        Tool_setBitOff(triggeredInputEvent, HB_INPUTBIT);

        // //Set a new time for the hbAlarmtrigger
        //TODO: check if I can affor to keep and do this in the FSM_f_HB

        // hbEPochTime = RTC_updateHBEPochTime(hbEPochTime, cfgStructArg);
    }

    rmsClassArg.set_inputEventCode(eventInputCode);
}


void FSM_executeFunction(Ezo_board& EZO_ORP, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]){
    bool debug=true;
    // Serial.print("SystemStatusRegister: ");
    // Serial.println(PMIC.readSystemStatusRegister(), BIN);

    uint8_t eventInputCode = rmsClassArg.get_inputEventCode();
    if (Tool_isBitOn(eventInputCode, WM_INPUTBIT)){
        // Serial.println("in if");
        if (debug){
            Serial.println("in FSM_executeFunction, WM");
            Serial.print("Current unix time = ");
            Serial.println(rtcClassArg.getEpoch());
            Serial.print("event Input Code before update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);

        }

        FSM_f_WM_EZO(EZO_ORP, rmsClassArg, rtcClassArg, cfgStructArg, dataFileName);

        Tool_setBitOff(&eventInputCode, WM_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        //We now want to check the battery situation
        Tool_setBitOn(&eventInputCode, BUP_INPUTBIT);

        if (debug){
            Serial.println("");
            Serial.print("event Input Code after update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
        }
    }

    if (Tool_isBitOn(eventInputCode, URA_INPUTBIT)){
        // Serial.println("in if");
        if (debug){
            Serial.println("in FSM_executeFunction, URA");
            Serial.print("Current unix time = ");
            Serial.println(rtcClassArg.getEpoch());
            Serial.print("event Input Code before update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
        }

        Serial.print("milis after interrupt wakup: ");
        Serial.println(millis());

        FSM_f_URA(EZO_ORP, rmsClassArg, rtcClassArg, cfgStructArg, dataFileName);
        Tool_setBitOff(&eventInputCode, URA_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        
        if (debug){

            Serial.print("event Input Code after update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
        }
    }
    if (Tool_isBitOn(eventInputCode, HB_INPUTBIT)){
        // Serial.println("in if");
        if (debug){
            Serial.println("in FSM_executeFunction, HB");
            Serial.print("Current unix time = ");
            Serial.println(rtcClassArg.getEpoch());
            Serial.print("event Input Code before update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
        }


        FSM_f_HB(rmsClassArg, cfgStructArg);
        Tool_setBitOff(&eventInputCode, HB_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        
        if (debug){
            Serial.print("event Input Code after update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
        }
    }
    if (Tool_isBitOn(eventInputCode, BUP_INPUTBIT)){
        FSM_f_BUP(rmsClassArg, cfgStructArg);
        Tool_setBitOff(&eventInputCode, BUP_INPUTBIT);
    }
    rmsClassArg.set_inputEventCode(eventInputCode);
    if (debug){
        // Serial.println("");
    }
}



void FSM_f_WM_EZO(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]){
    
    uint32_t currentTime = rtcClassArg.getEpoch();

    /* 
    - Different time from the wakeUp time.
    - Storing this value is useful for datalogging purpose
    */
    rmsClassArg.set_wmReadEPochTime(currentTime);
    Serial.print("rmsClassArg.get_wmReadEPochTime(): ");
    Serial.println(rmsClassArg.get_wmReadEPochTime());
    
    EZO_getEzoORPReading(ezoClassArg);

    float orpValue = ezoClassArg.get_last_received_reading();
    rmsClassArg.set_orpReading(orpValue);

    RMSState newState = FSM_decideState(ezoClassArg, cfgStructArg);
    rmsClassArg.set_rmsState(newState);
    // we now tell the device how long he may sleep for
    rmsClassArg.set_sleepPeriod();

    // // decide whether or not to send SMSs
    // rmsClassArg.set_inHistoryWindow();
    // rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wakeUpEPochTime()+rmsClassArg.get_sleepPeriod());
    // This solution would allow to minisme this drift. However, it would probably require to add a check to ensure the next wakup alarm time is later then the present time
    // TODO: add a check to ensure the next wakeup alarm is later then the present time

    // now we knwo the state the device is in, we can adjust for the appropriate sleep period
    // TODO: add a check just before going to sleep that the next wakeup time is in the future
    rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wmWakeUpEPochTime()+rmsClassArg.get_sleepPeriod());
    


    /* 
    - This has been placed literaly just before going to sleep. 
    This ensures we dont do twice the same thing, and that the right time is set for going to sleep
    - TODO: CHECK THIS IS BUG PROOF WITH THE COMMENT BEFORE GOING TO SLEEP
    */
    // rtcClassArg.setAlarmEpoch(rmsClassArg.get_nextWakeUpEPochTime());
    
    // check battery health
    FSM_setPowerSituation(rmsClassArg);


    bool writeSuccess = Data_saveDataPointToDataFile(rmsClassArg,
                        WM_INPUTBIT,
                        dataFileName);
    Serial.print("Was the write a success? ");
    Serial.println(writeSuccess);

    FSM_multipleAlarmManagement(rmsClassArg, cfgStructArg, currentTime, dataFileName);

    

    // if (rmsClassArg.get_rmsState() == UWQ || rmsClassArg.get_rmsState() == FWQ){
    // if (rmsClassArg.wm_canSendSMS(currentTime)){
    //     // perform thecount of UWQ/FWQ/SWQ eventsS
    //     Data_updateStateHistory(rmsClassArg, dataFileName);
    //     Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
    //     Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
    //     Serial.print("rmsClassArg.get_totalStateChanges()");
    //     Serial.println(rmsClassArg.get_totalStateChanges());
    //     //update percentage:
    //     rmsClassArg.set_stateHistoryPercentage(SWQ);
    //     rmsClassArg.set_stateHistoryPercentage(UWQ);
    //     rmsClassArg.set_stateHistoryPercentage(FWQ);
    //     float testFloat = 2/3;
    //     Serial.println(testFloat);
    //     SMS_wmSend(rmsClassArg);
    //     //TODO:  create a reset function
    //     rmsClassArg.reset_History();
    //     Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
    //     Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
    //     Serial.print("rmsClassArg.get_totalStateChanges()");
    //     Serial.println(rmsClassArg.get_totalStateChanges());
    // }
}


void FSM_f_URA(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]){
    uint32_t currentTime = rtcClassArg.getEpoch();
    // ToggleLED(ORANGELED_PIN);
    debugDisplay = 1;
    // read water value
    EZO_getEzoORPReading(ezoClassArg);

    float orpValue = ezoClassArg.get_last_received_reading();
    rmsClassArg.set_orpReading(orpValue);

    RMSState newState = FSM_decideState(ezoClassArg, cfgStructArg);

    rmsClassArg.set_rmsState(newState);

    // // we now tell the device how long he may sleep for
    // rmsClassArg.set_sleepPeriod();

    Serial.print("Battery Voltage from function: ");
    Serial.print(Battery_getBatteryVoltage());


    // check battery health
    FSM_setPowerSituation(rmsClassArg);

    Serial.print("Battery Voltage from struct: ");
    Serial.println(rmsClassArg.get_powerStructBatteryVoltage());
    // Serial.println(rmsClassArg.get_powerStructMember(1));
    // Serial.println(rmsClassArg.get_powerStructMember(2));
    // Serial.println(rmsClassArg.get_powerStructMember(3));


    //store value
    Serial.print("dataFileName: ");
    Serial.println(dataFileName);
    // bool writeSuccess = Data_saveDataPointToDataFile(rmsClassArg.get_wmReadEPochTime(),
    //                     rmsClassArg.get_orpReading(),
    //                     rmsClassArg.get_rmsState(),
    //                     URA_INPUTBIT,
    //                     rmsClassArg.get_powerStructBatteryVoltage(),
    //                     // rmsClassArg.get_powerStructUSBMode(),
    //                     rmsClassArg.get_powerStructStablePowerSupply(),
    //                     rmsClassArg.get_powerStructChargeStatus(),
    //                     dataFileName);
    bool writeSuccess = Data_saveDataPointToDataFile(rmsClassArg,
                        URA_INPUTBIT,
                        dataFileName);
    Serial.print("dataFileName: ");
    Serial.println(dataFileName);
    Serial.print("Was the write a success? ");
    Serial.println(writeSuccess);
    if (rmsClassArg.ura_canSendSMS(currentTime)){
        ToggleLED(ORANGELED_PIN);
        SMS_uraSend(rmsClassArg, cfgStructArg);
    }
    else{
        ToggleLED(BLUELED_PIN);
    }
}

void FSM_f_HB(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    //Set a new time for the hbAlarmtrigger
    hbEPochTime = RTC_updateHBEPochTime(hbEPochTime, cfgStructArg);
    SMS_hbSend(rmsClassArg, cfgStructArg);
    ToggleLED(BLUELED_PIN);
}

void FSM_f_BUP(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    uint32_t currentEpochTime = rmsClassArg.get_wmReadEPochTime();
    if (rmsClassArg.get_smsPowerStructIsStablePowerSupply() != rmsClassArg.get_powerStructStablePowerSupply()){
        SMS_BUPSendIsStablePowerSupply(rmsClassArg, cfgStructArg);

        rmsClassArg.set_smsPowerStructIsStablePowerSupply(rmsClassArg.get_powerStructStablePowerSupply(), currentEpochTime);
    }

    if (currentEpochTime > rmsClassArg.get_smsPowerStructEnergyLevelSMSSentEPochTime() + SMS_HW_BUP // this is to prevent spamming from flucutating changes
    && !rmsClassArg.get_powerStructStablePowerSupply() ){
        if (rmsClassArg.get_smsPowerStructBatteryEnergyLevelState() != rmsClassArg.get_powerStructBatteryELState()){
            SMS_BUPSendEnergyLevel(rmsClassArg, cfgStructArg);

            rmsClassArg.set_smsPowerStructBatteryEnergyLevelState(rmsClassArg.get_powerStructBatteryELState(), currentEpochTime);
        }
    }

    // time comparison: current vs previous sms sent time using the HW
        // check staus of current SMS sent vs real situation
    if (!rmsClassArg.get_powerStructStablePowerSupply()){
        if(rmsClassArg.get_powerStructBatteryELState() == rmsClassArg.criticalEL){
            rtc.disableAlarm();
            rtc.detachInterrupt();
            detachInterrupt(BUTTON_PIN);
            
            rmsClassArg.set_rmsState(SLEEP);
            // update rmsPowerState --> do this in FSM_setPowerSituation
            // LowPower.attachInterruptWakeup(digitalPinToInterrupt(PIN_PA27))
            // attach interrupt from BQ24195L



            // send SMS to deepSleep

        }
    }

}
// void FSM_f_BUP(rmsClass& rmsClassArg){
//     if (rmsClassArg.get_powerStructStablePowerSupply())
// }
// void FSM_multipleAlarmManagement(rmsClass& rmsClassArg, uint32_t currentTime){
//     Serial.print("AlarmSituation beofre update: ");
//     Serial.println(rmsClassArg.get_wmAlarmSituation());
//     rmsClassArg.update_wmAlarmSituation(currentTime);
//     Serial.print("AlarmSituation after update: ");
//     Serial.println(rmsClassArg.get_wmAlarmSituation());
//     if (rmsClassArg.get_wmAlarmSituation()>0){
//         // if (rmsClassArg.get_wmAlarmSituation()>1){//to save time
//             // perform thecount of UWQ/FWQ/SWQ eventsS
//             Data_updateStateHistory(rmsClassArg, dataFileName);
//             Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
//             Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
//             Serial.print("rmsClassArg.get_totalStateChanges()");
//             Serial.println(rmsClassArg.get_totalStateChanges());   
//             //update percentage:
//             rmsClassArg.set_stateHistoryPercentage(SWQ);
//             rmsClassArg.set_stateHistoryPercentage(UWQ);
//             rmsClassArg.set_stateHistoryPercentage(FWQ);
//             SMS_wmSend(rmsClassArg);
//             rmsClassArg.reset_History();
//             Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
//             Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
//             Serial.print("rmsClassArg.get_totalStateChanges()");
//             Serial.println(rmsClassArg.get_totalStateChanges());
//         // }
//     }
// }

void FSM_multipleAlarmManagement(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg, uint32_t currentTime, char dataFileName[]){
    rmsClassArg.set_wmCurrentAlarmEPochTime(currentTime);

    Serial.print("get_wmCurrentAlarmEPochTime(): ");
    Serial.println(rmsClassArg.get_wmCurrentAlarmEPochTime());
    Serial.print("get_wmLastAlarmEPochTime(): ");
    Serial.println(rmsClassArg.get_wmLastAlarmSMSEPochTime());
    Serial.print("Time difference: ");
    Serial.println(rmsClassArg.get_wmCurrentAlarmEPochTime()-rmsClassArg.get_wmLastAlarmSMSEPochTime());
    Serial.print("Allowed interval: ");
    Serial.println(rmsClassArg.get_wmAllowedIntervalBetweenSMS());
    Serial.print("second condition: ");
    bool test = (rmsClassArg.get_wmCurrentAlarmEPochTime()-rmsClassArg.get_wmLastAlarmSMSEPochTime() > rmsClassArg.get_wmAllowedIntervalBetweenSMS());
    Serial.println (test);
    Serial.print("AlarmSituation beofre update: ");
    Serial.println(rmsClassArg.get_wmAlarmSituation());


    if ((rmsClassArg.get_wmAlarmSituation() == rmsClass::NOANOMALIES) 
    && (rmsClassArg.get_rmsState() == UWQ || rmsClassArg.get_rmsState() == FWQ)){
        
        rmsClassArg.set_wmLastAlarmSMSEPochTime(rmsClassArg.get_wmCurrentAlarmEPochTime());
        
        rmsClassArg.set_wmAlarmSituation(rmsClass::FIRSTANOMALY);
        
        SMS_wmSend(rmsClassArg, cfgStructArg);
    }
    // TODO: instead of storing the and using some extra space, can directly use the value from cfg for wmSMSInterval
    else if ((rmsClassArg.get_wmAlarmSituation() == rmsClass::HWANOMALIES) 
    && (rmsClassArg.get_wmCurrentAlarmEPochTime()-rmsClassArg.get_wmLastAlarmSMSEPochTime() > rmsClassArg.get_wmAllowedIntervalBetweenSMS())){
        
        rmsClassArg.set_wmLastAlarmSMSEPochTime(rmsClassArg.get_wmCurrentAlarmEPochTime());
        
        rmsClassArg.set_wmAlarmSituation(rmsClass::HWANOMALIES);
        
        // perform thecount of UWQ/FWQ/SWQ eventsS
        Data_updateStateHistory(rmsClassArg, dataFileName);
        Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
        Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
        Serial.print("rmsClassArg.get_totalStateChanges()");
        Serial.println(rmsClassArg.get_totalStateChanges());

        //update percentage:
        rmsClassArg.set_stateHistoryPercentage(SWQ);
        rmsClassArg.set_stateHistoryPercentage(UWQ);
        rmsClassArg.set_stateHistoryPercentage(FWQ);

        if (rmsClassArg.get_rmsState() == SWQ){ // THIS is where we can tune the settings to stop the alarm sending
            rmsClassArg.set_wmAlarmSituation(rmsClass::NORMALOCCURENCE);
        }

        SMS_wmSend(rmsClassArg, cfgStructArg);

        rmsClassArg.reset_History();
        
        Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
        Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
        Serial.print("rmsClassArg.get_totalStateChanges()");
        Serial.println(rmsClassArg.get_totalStateChanges());
    }
    Serial.print("AlarmSituation after update: ");
    Serial.println(rmsClassArg.get_wmAlarmSituation());
    
}

RMSState FSM_decideState(Ezo_board& ezoORPClassArg, ConfigurationStruct cfgStructArg){
    RMSState state = UWQ;
    if (ezoORPClassArg.get_error() == Ezo_board::SUCCESS){
        state = FSM_implementMLDecision(ezoORPClassArg, cfgStructArg);

    }
    else {
        state = FWQ;
    }
    return state;
}

RMSState FSM_implementMLDecision(Ezo_board& ezoORPClassArg, ConfigurationStruct cfgStructArg){
    // TODO: find a better way to easily tune the threshold
    // TODO: allow the ORPValue threshold to be set at initialisation

    RMSState state = UWQ;
    float orpValue = ezoORPClassArg.get_last_received_reading();
    // CONFIG
    if (orpValue > cfgStructArg.logitThreshold) {
        state = SWQ;
    }
    else if (orpValue <= cfgStructArg.logitThreshold){
        state = UWQ;
    }

    // TODO: need to deal with faulty reading as well
    return state;
}


void FSM_setPowerSituation(rmsClass& rmsClassArg){
    // CHeck battery health, and store into the RMS State class
    // this function also updates the energy level band
    rmsClassArg.set_powerStructBatteryVoltage(Battery_getBatteryVoltage());
    rmsClassArg.set_powerStructStablePowerSupply(Battery_getIsStablePowerSupply());
    rmsClassArg.set_powerStructChargeStatus(Battery_getChargeStatus());
    Serial.println("PowerSituation");
    Serial.println("value from Battery function");
    Serial.print("Battery_getBatteryVoltage: ");
    Serial.println(Battery_getBatteryVoltage());
    Serial.print("Battery_getStablePowerSupply: ");
    Serial.println(Battery_getIsStablePowerSupply(), BIN);
    Serial.print("Battery_getChargeStatus: ");
    Serial.println(Battery_getChargeStatus(), BIN);
    Serial.println("value from powerStruct function");
    Serial.print("rmsClassArg.get_powerStructBatteryVoltage: ");
    Serial.println(rmsClassArg.get_powerStructBatteryVoltage());
    Serial.print("rmsClassArg.get_powerStructUSBMode: ");
    Serial.println(rmsClassArg.get_powerStructStablePowerSupply(), BIN);
    Serial.print("rmsClassArg.get_powerStructChargeStatus: ");
    Serial.println(rmsClassArg.get_powerStructChargeStatus(), BIN);

}







// void FSM_updateState(Ezo_board* classArg, RMSState* currentState){
//     float ORPValue = classArg->get_last_received_reading();
//     if (ORPValue > 230) {
//         digitalWrite(REDLED_PIN, LOW);
//         digitalWrite(GREENLED_PIN, HIGH);
//         rmsState = SWQ;
//     }
//     else 
//     {
//         digitalWrite(REDLED_PIN, HIGH);
//         digitalWrite(GREENLED_PIN, LOW);
//     }
// }

// void FSM_waterMonitoring(char ORPData_[]){
//     bool debug = false;
//     uint8_t commandLength = 1;
//     char ORPCommand[commandLength] = {'r'};
//     I2c_sendReceiveORP(ORPData_, ORPCommand, commandLength);
//     if (debug){
//         Serial.print("ORP value: ");
//         Serial.println(ORPData_);
//     }
// }

// void FSM_getEzoWaterReading(Ezo_board* classArg){
//     classArg->send_read_cmd();
//     delay(815); // delay required for reading command
//     // TODO: eventually, will need to create my own fuction that only reads the values and that can then be used to store
//     receive_and_print_reading(*classArg);
//     // put device to sleep
//     classArg->send_cmd("Sleep");
// }



void FSM_initRMS(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    uint32_t currentEPochTime = rtc.getEpoch();
    // write values from config file into the rmsClass
    rmsClassArg.set_SWQSleepPeriod(cfgStructArg.swqSleepPeriod);
    rmsClassArg.set_UWQSleepPeriod(cfgStructArg.uwqSleepPeriod);
    rmsClassArg.set_FWQSleepPeriod(cfgStructArg.fwqSleepPeriod);

    rmsClassArg.set_URAallowedIntervalBetweenSMS(cfgStructArg.uraSMSInterval);
    rmsClassArg.set_wmAllowedIntervalBetweenSMS(cfgStructArg.wmSMSInterval);

    // Tell the device to go into a wm function
    rmsClassArg.set_inputEventCode(0b1);

    // as if at initialisation the device has just woken up
    
    rmsClassArg.set_wakeUpEPochTime(currentEPochTime);
    rmsClassArg.set_wmWakeUpEPochTime(currentEPochTime);


    // update the power situation in the rmsClass
    FSM_setPowerSituation(rmsClassArg);

    // consider that the operator knows the power situation on startup
    rmsClassArg.init_smsPowerStruct(currentEPochTime);

    // Ensure the state history is back to zero:
    rmsClassArg.reset_History();

    //ensure that the HB alarm is in the future (might not be if the device was in deepsleep, and had to wakeup after the HB would have occured)
    if(RTC_setUpHB(cfgStructArg)){
        Serial.println("HBtime set OK.");
    }
    else{
        rmsClassArg.set_rmsState(RTC_FAILEDHBSET);
    }

    
}
