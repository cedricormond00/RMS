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



// debug boolean
/* 
Some functions such as 
- 
- 
have an additional debug boolean, so as to be more granular
*/
bool debugDisplay = 1;

bool debug_FSM = false;

void FSM_initRMS(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    uint32_t currentEPochTime = rtc.getEpoch();
    // Write values from config file into the rmsClass
    rmsClassArg.set_SWQSleepPeriod(cfgStructArg.swqSleepPeriod);
    rmsClassArg.set_UWQSleepPeriod(cfgStructArg.uwqSleepPeriod);
    rmsClassArg.set_FWQSleepPeriod(cfgStructArg.fwqSleepPeriod);

    rmsClassArg.set_URAallowedIntervalBetweenSMS(cfgStructArg.uraSMSInterval);
    rmsClassArg.set_wmAllowedIntervalBetweenSMS(cfgStructArg.wmSMSInterval);

    // Let the machine know it must perform the Water Monitoring function right now
    rmsClassArg.set_inputEventCode(0b1);

    // As if at initialisation the device has just woken up
    rmsClassArg.set_wakeUpEPochTime(currentEPochTime);
    rmsClassArg.set_wmWakeUpEPochTime(currentEPochTime);

    // Update the power situation in the rmsClass
    FSM_setPowerSituation(rmsClassArg);

    // Consider that the operator knows the power situation on startup, so no need for them to receive an SMS
    rmsClassArg.init_smsPowerStruct(currentEPochTime);

    // Ensure the state history is back to zero:
    rmsClassArg.reset_History();

    /* Ensure that the HB alarm is in the future 
    (might not be if the device was in deepsleep, and had to wakeup after the HB would have occured) */
    if(RTC_setUpHB(cfgStructArg)){
        Serial.println("HBtime set OK.");
    }
    else{
        rmsClassArg.set_rmsState(RTC_FAILEDHBSET);
    }
}


void FSM_updateInputEventCode(rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, volatile uint8_t* triggeredInputEvent)
{

    bool debug=true;
    uint8_t inputEvenCode = rmsClassArg.get_inputEventCode();

    if (Tool_isBitOn(*triggeredInputEvent, WM_INPUTBIT)) 
    {
        if (debug && debug_FSM){
            Serial.println("in FSM_updateInputEventCode, WM");
            Serial.print("inputEvenCode before update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
            Serial.print("triggeredInputEvent before update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }
        uint32_t currentTime = rtcClassArg.getEpoch();
        char buf[256];

        Tool_stringTime(currentTime, buf);
        if (debug && debug_FSM){
            Serial.print("Current unix time: ");
            Serial.println(currentTime);
            Serial.print("Current unix timestamp: ");
            Serial.println(buf);
            Serial.print("rmsClassArg.get_wakeUpEPochTime(): ");
            Serial.println(rmsClassArg.get_wakeUpEPochTime());
            Serial.print("rmsClassArg.get_wmWakeUpEPochTime(): ");
            Serial.println(rmsClassArg.get_wmWakeUpEPochTime());
        }



        /*
        - Store the time at which the itnernal clock of the RMS matched the alarm
        - This usually occurs when the devie is asleep, and hence wakes the device up
        - Keeping this variable is very important: it allows to know at what time the device will need to wakeup next time
        - UPGRADE: a bit amiguous: the wm in this case refers to the watermonitoring action only (that fo checking the water, on a specific time).
        For clarity, we should differentiate between the action of simply monitoring the water quality (from URA or WM), and monitotinrg the water quality as a function (execited only from WM)
        */
        rmsClassArg.set_wmWakeUpEPochTime(alarmMatchEPochTime);


        Tool_setBitOn(&inputEvenCode, WM_INPUTBIT);// code for WM function

        // reset the triggered input event tracker
        Tool_setBitOff(triggeredInputEvent, WM_INPUTBIT);

         if (debug && debug_FSM){
            Serial.print("inputEvenCode after update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
            Serial.print("triggeredInputEvent after update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }

    }

    if (Tool_isBitOn(*triggeredInputEvent, URA_INPUTBIT)) 
    {
        if (debug && debugDisplay && debug_FSM){
            Serial.println("in FSM_updateInputEventCode, URA");
            Serial.print("inputEvenCode before update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
            Serial.print("triggeredInputEvent before update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }

        //because we haven't yet reached the 3 seconds
        Tool_setBitOn(&inputEvenCode, URA_WAIT_INPUTBIT);// This prevents to go back to sleep
        
        // check the button is still pressed 
        if (digitalRead(BUTTON_PIN)==LOW){
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= 500){
                ToggleLED(YELLOWLED_PIN);
                previousMillis = currentMillis;
            }
            if (currentMillis>=(millisOnExternalWakeUp + cfgStructArg.uraPressDuration)){
                digitalWrite(YELLOWLED_PIN, LOW);
                //inform the RMS the URA function may be performed
                Tool_setBitOn(&inputEvenCode, URA_INPUTBIT);
                Tool_setBitOff(&inputEvenCode, URA_WAIT_INPUTBIT);

                // reset the triggered input event tracker
                Tool_setBitOff(triggeredInputEvent, URA_INPUTBIT);
            }
        }
        else { // the button has been released before the 3 seconds have
            // no more URA alarm
            Tool_setBitOff(triggeredInputEvent, URA_INPUTBIT);
            // no need to inform that we shall wait for 3 seconds
            Tool_setBitOff(&inputEvenCode, URA_WAIT_INPUTBIT);
            digitalWrite(YELLOWLED_PIN, LOW);

        }

        
        if (debug && debugDisplay){
            Serial.print("inputEvenCode after update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
            Serial.print("triggeredInputEvent after update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
            debugDisplay = 0;
        }
    }
    if (Tool_isBitOn(*triggeredInputEvent, HB_INPUTBIT)){
        Tool_setBitOn(&inputEvenCode, HB_INPUTBIT);// code for WM function

        // reset the triggered input event tracker
        Tool_setBitOff(triggeredInputEvent, HB_INPUTBIT);
    }

    rmsClassArg.set_inputEventCode(inputEvenCode);
}


void FSM_executeFunction(Ezo_board& EZO_ORP, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]){
    //boolean for local function debugging
    bool debug=true;

    // Find the function to execute, using the inputEventCode, set by FSM_updateInputEventCode
    // TODO: clean if we use inputEventCode (I prefer this one) or inputEvenCode
    uint8_t inputEvenCode = rmsClassArg.get_inputEventCode();

    // execute a water monitoring
    if (Tool_isBitOn(inputEvenCode, WM_INPUTBIT)){
        if (debug && debug_FSM){
            Serial.println("in FSM_executeFunction, WM");
            Serial.print("Current unix time = ");
            Serial.println(rtcClassArg.getEpoch());
            Serial.print("event Input Code before update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
        }
        
        //perform the function
        FSM_f_WM_EZO(EZO_ORP, rmsClassArg, rtcClassArg, cfgStructArg, dataFileName);

        // set the input event code bit off
        Tool_setBitOff(&inputEvenCode, WM_INPUTBIT); // because inputEvenCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        //We now want to perform the back-up power check (inform the operator and user if required)
        //TODO: trigger appropriate LEDs
        Tool_setBitOn(&inputEvenCode, BUP_INPUTBIT);

        if (debug && debug_FSM){
            Serial.println("");
            Serial.print("event Input Code after update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
        }
    }

    if (Tool_isBitOn(inputEvenCode, URA_INPUTBIT)){
        // Serial.println("in if");
        if (debug && debug_FSM){
            Serial.println("in FSM_executeFunction, URA");
            Serial.print("Current unix time = ");
            Serial.println(rtcClassArg.getEpoch());
            Serial.print("event Input Code before update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
            Serial.print("milis after interrupt wakup: ");
            Serial.println(millis());
        }

        //perform the function
        FSM_f_URA(EZO_ORP, rmsClassArg, rtcClassArg, cfgStructArg, dataFileName);
        
        // set the input event code bit off
        Tool_setBitOff(&inputEvenCode, URA_INPUTBIT); // because inputEvenCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        
        if (debug && debug_FSM){

            Serial.print("event Input Code after update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
        }
    }
    if (Tool_isBitOn(inputEvenCode, HB_INPUTBIT)){

        if (debug && debug_FSM){
            Serial.println("in FSM_executeFunction, HB");
            Serial.print("Current unix time = ");
            Serial.println(rtcClassArg.getEpoch());
            Serial.print("event Input Code before update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
        }

        //perform the function
        FSM_f_HB(rmsClassArg, cfgStructArg);

        //HERE

        // set the input event code bit off
        Tool_setBitOff(&inputEvenCode, HB_INPUTBIT); // because inputEvenCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        
        if (debug && debug_FSM){
            Serial.print("event Input Code after update: ");
            Serial.print(inputEvenCode, BIN);
            Serial.print(", ");
            Serial.println(inputEvenCode);
        }
    }
    if (Tool_isBitOn(inputEvenCode, BUP_INPUTBIT)){
        FSM_f_BUP(rmsClassArg, cfgStructArg);
        Tool_setBitOff(&inputEvenCode, BUP_INPUTBIT);
    }
    rmsClassArg.set_inputEventCode(inputEvenCode);
    if (debug){
        // Serial.println("");
    }
}



void FSM_f_WM_EZO(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]){
    bool debug = false;
    // Different time from the wakeUp time.
    uint32_t currentTime = rtcClassArg.getEpoch();
    
    //Storing this value is useful for datalogging purpose
    rmsClassArg.set_wmReadEPochTime(currentTime);
    if (debug && debug_FSM){
        Serial.print("in WM function, rmsClassArg.get_wmReadEPochTime(): ");
        Serial.println(rmsClassArg.get_wmReadEPochTime());
    }
    
    // get and print the ORP reading from the ORP sensor
    EZO_getEzoORPReading(ezoClassArg);          // value is stored in the ezoClassArg object

    float orpValue = ezoClassArg.get_last_received_reading();

    // store the ORP value inside the rmsClassArg object
    rmsClassArg.set_orpReading(orpValue);

    // based on the ORP value and the configuration settings, decide on the rms State
    RMSState newState = FSM_decideState(ezoClassArg, cfgStructArg);

    // update the rmsClassArg object with the ndew evaluated state
    rmsClassArg.set_rmsState(newState);

    // now decide how long the device may sleep for
    rmsClassArg.set_sleepPeriod();

    // now also evalute at what time the device should then wake up at
    // before going to sleep, you will still need to set the alarm time for wakeup
    rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wmWakeUpEPochTime()+rmsClassArg.get_sleepPeriod());

    // check battery health
    FSM_setPowerSituation(rmsClassArg);


    bool writeSuccess = Data_saveDataPointToDataFile(rmsClassArg,
                        WM_INPUTBIT,
                        dataFileName);
    if (debug && debug_FSM){
        Serial.print("Was the write a success? ");
        Serial.println(writeSuccess);
    }
    FSM_multipleAlarmManagement(rmsClassArg, cfgStructArg, currentTime, dataFileName);
}


void FSM_f_URA(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]){
    // for debugging purpose
    debugDisplay = 1;
    bool debug = true;

    // time of URA miught be different from Wakeup, especially if the device woke up, performed a WM. Also consider that 3 secondsat least happen between wakeup and URA function
    uint32_t currentTime = rtcClassArg.getEpoch();

    //Storing this value is useful for datalogging purpose
    // recall that wmRead is the time at which a water sampling occured, rgardless of the event causing it
    rmsClassArg.set_wmReadEPochTime(currentTime);
    if (debug && debug_FSM){
        Serial.print("in URA function, rmsClassArg.get_wmReadEPochTime(): ");
        Serial.println(rmsClassArg.get_wmReadEPochTime());
    }


    // get and print the ORP reading from the ORP sensor
    EZO_getEzoORPReading(ezoClassArg);

    float orpValue = ezoClassArg.get_last_received_reading();
    
    // store the ORP value inside the rmsClassArg object
    rmsClassArg.set_orpReading(orpValue);

    // based on the ORP value and the configuration settings, decide on the rms State
    RMSState newState = FSM_decideState(ezoClassArg, cfgStructArg);
   
    // update the rmsClassArg object with the ndew evaluated state
    rmsClassArg.set_rmsState(newState);

    // // we now tell the device how long he may sleep for
    // rmsClassArg.set_sleepPeriod();

    //TODO: check why we dont have to evaluate sleep period and next wakeup time


    // check battery health
    FSM_setPowerSituation(rmsClassArg);


    //store value
    bool writeSuccess = Data_saveDataPointToDataFile(rmsClassArg,
                        URA_INPUTBIT,
                        dataFileName);
    if (debug && debug_FSM){
        Serial.print("dataFileName: ");
        Serial.println(dataFileName);
        Serial.print("Was the write a success? ");
        Serial.println(writeSuccess);
    }

    // message spamming management
    if (rmsClassArg.ura_canSendSMS(currentTime)){
        if (debug && debug_FSM){
            ToggleLED(ORANGELED_PIN);
        }
        SMS_uraSend(rmsClassArg, cfgStructArg);
    }
    else{
        if (debug && debug_FSM){
            ToggleLED(BLUELED_PIN);
        }
    }
}


void FSM_f_HB(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    bool debug = true;
    //Set a new time for the hbAlarmtrigger
    hbEPochTime = RTC_updateHBEPochTime(hbEPochTime, cfgStructArg);
    //send the HB SMS
    SMS_hbSend(rmsClassArg, cfgStructArg);
    if (debug && debug_FSM){
        ToggleLED(BLUELED_PIN);
    }
    
}



void FSM_f_BUP(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    /* 
    recall _WmReadTime is updated any time a modifiucation to the rmsClass is peformed, to vlaue related to the current state.
    As such, since the BUP is performed after the wm input event, we must use the wmReadTime, as the time at which the BUP occured
    */
    // uint32_t currentEpochTime = rmsClassArg.get_wmReadEPochTime();

    //for moreflexibility, ocould justuse the rtc.getEpoch wherever currentTuime is required
    uint32_t currentEpochTime = rtc.getEpoch();

    /* 
    check whether the power stability situation is on the same state
    as the sms sent to inform on power stabilit. If they are not the same, enter 
    (falling/rising edge scheme)
    */
    if (rmsClassArg.get_smsPowerStructIsStablePowerSupply() != rmsClassArg.get_powerStructStablePowerSupply()){
        /* Set
        - what time we are sending an SMS for BUP Stable power supply situation
        - update the sms BUP stable power supply state to the real power supply situation
        */
        rmsClassArg.set_smsPowerStructIsStablePowerSupply(rmsClassArg.get_powerStructStablePowerSupply(), currentEpochTime);

        // send the SMS
        SMS_BUPSendIsStablePowerSupply(rmsClassArg, cfgStructArg);


        // //TODO: check if we can exchange both following line and then change the time we use in the SMS sending
        // SMS_BUPSendIsStablePowerSupply(rmsClassArg, cfgStructArg);

        // rmsClassArg.set_smsPowerStructIsStablePowerSupply(rmsClassArg.get_powerStructStablePowerSupply(), currentEpochTime);
    }
    /* 
    ONly send an SMS if
    - to prevent spamming (from hysterisis), only send an EL update, 
    if the device is past the SMS_HW_BUP (time window)
    - unstable power supply (ie, not running on the mains)
    */
    if (currentEpochTime > rmsClassArg.get_smsPowerStructEnergyLevelSMSSentEPochTime() + SMS_HW_BUP // this is to prevent spamming from flucutating changes
    && !rmsClassArg.get_powerStructStablePowerSupply() ){
        if (rmsClassArg.get_smsPowerStructBatteryEnergyLevelState() != rmsClassArg.get_powerStructBatteryELState()){
            /* Set
            - what time we are sending an SMS for BUP Energy Level situation
            - update the sms BUP enery Level state to the real energy level situation
            */
            rmsClassArg.set_smsPowerStructBatteryEnergyLevelState(rmsClassArg.get_powerStructBatteryELState(), currentEpochTime);

            //send SMS for energy level update
            SMS_BUPSendEnergyLevel(rmsClassArg, cfgStructArg);

        }
    }
    /*
    - not on mains 
    - critical energy level
    */
    if (!rmsClassArg.get_powerStructStablePowerSupply()){
        if(rmsClassArg.get_powerStructBatteryELState() == rmsClassArg.criticalEL){
            // Disable the RTC alarm triggering.
            rtc.disableAlarm();

            // Detach interrupt action on alarm match.
            rtc.detachInterrupt();
            // Detach the interrupt on button press
            detachInterrupt(BUTTON_PIN);

            rmsClassArg.set_rmsState(SLEEP);
            // update rmsPowerState --> do this in FSM_setPowerSituation
            // LowPower.attachInterruptWakeup(digitalPinToInterrupt(PIN_PA27))
            // attach interrupt from BQ24195L



            // send SMS to deepSleep

        }
    }
}


RMSState FSM_decideState(Ezo_board& ezoORPClassArg, ConfigurationStruct cfgStructArg){
    RMSState state = UWQ;
    //Could successfully read a value from the sensor
    if (ezoORPClassArg.get_error() == Ezo_board::SUCCESS){
        state = FSM_implementMLDecision(ezoORPClassArg, cfgStructArg);
    }
    // problem occured while reading the sensor
    else {
        state = FWQ;
    }
    return state;
}

RMSState FSM_implementMLDecision(Ezo_board& ezoORPClassArg, ConfigurationStruct cfgStructArg){
    // TODO: allow the ORPValue threshold to be set at initialisation
    // UPGRADE: tune here the decision making criteria
    RMSState state = UWQ;
    float orpValue = ezoORPClassArg.get_last_received_reading();
    // test for threshold value
    if (orpValue > cfgStructArg.logitThreshold) {
        state = SWQ;
    }
    else if (orpValue <= cfgStructArg.logitThreshold){
        state = UWQ;
    }
    return state;
}


void FSM_setPowerSituation(rmsClass& rmsClassArg){
    bool debug = true;
    // Check battery health, and store into the RMS State class
    // this function also updates the energy level band
    rmsClassArg.set_powerStructBatteryVoltage(Battery_getBatteryVoltage());
    rmsClassArg.set_powerStructStablePowerSupply(Battery_getIsStablePowerSupply());
    rmsClassArg.set_powerStructChargeStatus(Battery_getChargeStatus());

    if (debug_FSM && debug){
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
        Serial.print("rmsClassArg.get_powerStructStablePowerSupply: ");
        Serial.println(rmsClassArg.get_powerStructStablePowerSupply(), BIN);
        Serial.print("rmsClassArg.get_powerStructChargeStatus: ");
        Serial.println(rmsClassArg.get_powerStructChargeStatus(), BIN);
    }
}

void FSM_multipleAlarmManagement(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg, uint32_t currentTime, char dataFileName[]){
    bool debug = false;
    rmsClassArg.set_wmCurrentAlarmEPochTime(currentTime);
    if (debug && debug_FSM){
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
        Serial.print("WM alarm situation before update: ");
        Serial.println(rmsClassArg.get_wmAlarmSituation());
    }
    
    /* 
    The alarm situation is in no anomaly mode (before the water was Safe)
    but the rms has just detected an UWQ of FWQ.
    */
    if ((rmsClassArg.get_wmAlarmSituation() == rmsClass::NOANOMALIES) 
    && (rmsClassArg.get_rmsState() == UWQ || rmsClassArg.get_rmsState() == FWQ)){
        // we will send an SMS; so we update the lastAlarmSMSEPochTime
        rmsClassArg.set_wmLastAlarmSMSEPochTime(rmsClassArg.get_wmCurrentAlarmEPochTime());
        
        // this is the first anaomly detected
        rmsClassArg.set_wmAlarmSituation(rmsClass::FIRSTANOMALY);
        
        // send an SMS
        SMS_wmSend(rmsClassArg, cfgStructArg);
    }
    // TODO: instead of storing the and using some extra space, can directly use the value from cfg for wmSMSInterval
    /* 
    The alarm situation is in History Window (containing anomalies (UWQ or FWQ))
    and, we are now at the end of the History Window duration: we should update the operator on the situation
    */
    else if ((rmsClassArg.get_wmAlarmSituation() == rmsClass::HWANOMALIES) 
    && (rmsClassArg.get_wmCurrentAlarmEPochTime()-rmsClassArg.get_wmLastAlarmSMSEPochTime() > rmsClassArg.get_wmAllowedIntervalBetweenSMS())){
        // we will send an SMS; so we update the lastAlarmSMSEPochTime
        rmsClassArg.set_wmLastAlarmSMSEPochTime(rmsClassArg.get_wmCurrentAlarmEPochTime());
        
        // We are still in a history window: assumes the criteria for terminating a HW has not been reached yet
        rmsClassArg.set_wmAlarmSituation(rmsClass::HWANOMALIES);
        
        // perform thecount of UWQ/FWQ/SWQ eventsS
        rmsClassArg.set_stateHistorySuccess(Data_updateStateHistory(rmsClassArg, dataFileName));
        if (debug && debug_FSM){
            Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
            Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
            Serial.print("rmsClassArg.get_totalStateChanges()");
            Serial.println(rmsClassArg.get_totalStateChanges());
        }

        if (rmsClassArg.get_stateHistorySuccess()){
            // final percentage update:
            rmsClassArg.set_stateHistoryPercentage(SWQ);
            rmsClassArg.set_stateHistoryPercentage(UWQ);
            rmsClassArg.set_stateHistoryPercentage(FWQ);
        }
        /*
        If the final data point lead to a SWQ, we assume in this version that the device is now in SWQ, and should exit the HW
        UPGRADE: THIS is where we can tune the settings to stop the alarm sending
        */
        if (rmsClassArg.get_rmsState() == SWQ){ 
            rmsClassArg.set_wmAlarmSituation(rmsClass::NORMALOCCURENCE);
        }

        SMS_wmSend(rmsClassArg, cfgStructArg);

        rmsClassArg.reset_History();
        if (debug && debug_FSM){
            Serial.println("after rmsClassArg.reset_History");
            Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
            Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
            Serial.print("rmsClassArg.get_totalStateChanges()");
            Serial.println(rmsClassArg.get_totalStateChanges());
        }
    }
    if (debug && debug_FSM){
        Serial.print("AlarmSituation after update: ");
        Serial.println(rmsClassArg.get_wmAlarmSituation());
    }
}

