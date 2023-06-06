#include <Arduino.h>
#include <ArduinoLowPower.h>


#include <iot_cmd.h>
#include <Ezo_i2c_util.h>  

#include "FSM.h"

#include "Constant.h"
#include "Global.h"


#include "Tool.h"

#include "LED.h"
#include "I2c.h"
#include "RTC.h"
#include "Data.h"
#include "EZO.h"
#include "SMS.h"

bool debugDisplay = 1;


void FSM_updateInputEventCode(rmsClass& rmsClassArg, RTCZero& rtcClassArg, volatile uint8_t* triggeredInputEvent)
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

        // // Check to see if the alarm match occured at a time when the device was not asleep
        // // alarm occured when device not asleep 
        // // -> artificially use current time
        // if (Tool_isBitOn(*triggeredInputEvent, 0b11111110) || Tool_isBitOn(rmsClassArg.get_inputEventCode(), 0b11111110)){ //alarm occured when the device was not asleep
        //     rmsClassArg.set_wmWakeUpEPochTime(currentTime); //+1
        //     Serial.print("Alarm match occured when device awake: ");
        //     Serial.print("rmsClassArg.get_wmWakeUpEPochTime(): ");
        //     Serial.println(rmsClassArg.get_wmWakeUpEPochTime());
        // }
        // // alarm occured when the device was asleep
        // // -> use the time at which the device wokeup
        // else{ 
        //     rmsClassArg.set_wmWakeUpEPochTime(rmsClassArg.get_wakeUpEPochTime());
        //     Serial.print("Alarm match occured when device asleep: ");
        //     Serial.print("rmsClassArg.get_wmWakeUpEPochTime(): ");
        //     Serial.println(rmsClassArg.get_wmWakeUpEPochTime());
        // }

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
            if (currentMillis>=(millisOnExternalWakeUp + 3000)){
                digitalWrite(YELLOWLED_PIN, LOW);
                //inform the RMS the URA function may be performed
                Tool_setBitOn(&eventInputCode, URA_INPUTBIT);// code for UserraisedAlarm function
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

    rmsClassArg.set_inputEventCode(eventInputCode);
}


void FSM_executeFunction(Ezo_board& EZO_ORP, rmsClass& rmsClassArg, RTCZero& rtcClassArg){
    bool debug=true;

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

        FSM_f_WM_EZO(EZO_ORP, rmsClassArg, rtcClassArg);

        Tool_setBitOff(&eventInputCode, WM_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff

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

        FSM_f_URA(EZO_ORP, rmsClassArg, rtcClassArg);
        Tool_setBitOff(&eventInputCode, URA_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        
        if (debug){

            Serial.print("event Input Code after update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
        }
    }

    rmsClassArg.set_inputEventCode(eventInputCode);
    if (debug){
        // Serial.println("");
    }
}



void FSM_f_WM_EZO(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg){
    
    uint32_t currentTime = rtcClassArg.getEpoch();

    rmsClassArg.set_wmReadEPochTime(currentTime);
    Serial.print("rmsClassArg.get_wmReadEPochTime(): ");
    Serial.println(rmsClassArg.get_wmReadEPochTime());
    
    EZO_getEzoORPReading(ezoClassArg);

    float orpValue = ezoClassArg.get_last_received_reading();
    rmsClassArg.set_orpReading(orpValue);

    RMSState newState = FSM_decideState(ezoClassArg);
    rmsClassArg.set_rmsState(newState);

    // // decide whether or not to send SMSs
    // rmsClassArg.set_inHistoryWindow();

    // rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wakeUpEPochTime()+rmsClassArg.get_sleepPeriod());
    // This solution would allow to minisme this drift. However, it would probably require to add a check to ensure the next wakup alarm time is later then the present time
    // TODO: add a check to ensure the next wakeup alarm is later then the present time


    rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wmWakeUpEPochTime()+rmsClassArg.get_sleepPeriod());
    
    // rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wmReadEPochTime()+rmsClassArg.get_sleepPeriod());

    // should be added literaly just before going to sleep
    //TODO: CHECK THIS IS BUG PROOF WITH THE COMMENT BEFORE GOING TO SLEEP
    rtcClassArg.setAlarmEpoch(rmsClassArg.get_nextWakeUpEPochTime());
    Data_saveDataPointToDataFile(rmsClassArg.get_wmReadEPochTime(),
                        rmsClassArg.get_orpReading(),
                        rmsClassArg.get_rmsState(),
                        WM_INPUTBIT,
                        dataFileName);
    FSM_multipleAlarmManagement(rmsClassArg, currentTime);
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


void FSM_f_URA(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg){
    uint32_t currentTime = rtcClassArg.getEpoch();
    // ToggleLED(ORANGELED_PIN);
    debugDisplay = 1;
    // read water value
    EZO_getEzoORPReading(ezoClassArg);

    float orpValue = ezoClassArg.get_last_received_reading();
    rmsClassArg.set_orpReading(orpValue);

    RMSState newState = FSM_decideState(ezoClassArg);

    rmsClassArg.set_rmsState(newState);

    //store value
    Data_saveDataPointToDataFile(currentTime,
                        rmsClassArg.get_orpReading(),
                        rmsClassArg.get_rmsState(),
                        URA_INPUTBIT,
                        dataFileName);
    if (rmsClassArg.ura_canSendSMS(currentTime)){
        ToggleLED(ORANGELED_PIN);
        SMS_uraSend(rmsClassArg);
    }
    else{
        ToggleLED(BLUELED_PIN);
    }
}

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

void FSM_multipleAlarmManagement(rmsClass& rmsClassArg, uint32_t currentTime){
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


    if ((rmsClassArg.get_wmAlarmSituation() == 0) 
    && (rmsClassArg.get_rmsState() == UWQ || rmsClassArg.get_rmsState() == FWQ)){
        
        rmsClassArg.set_wmLastAlarmSMSEPochTime(rmsClassArg.get_wmCurrentAlarmEPochTime());
        
        rmsClassArg.set_wmAlarmSituation(1);
        
        SMS_wmSend(rmsClassArg);
    }
    else if ((rmsClassArg.get_wmAlarmSituation() == 2) 
    && (rmsClassArg.get_wmCurrentAlarmEPochTime()-rmsClassArg.get_wmLastAlarmSMSEPochTime() > rmsClassArg.get_wmAllowedIntervalBetweenSMS())){
        
        rmsClassArg.set_wmLastAlarmSMSEPochTime(rmsClassArg.get_wmCurrentAlarmEPochTime());
        
        rmsClassArg.set_wmAlarmSituation(2);
        
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
            rmsClassArg.set_wmAlarmSituation(3);
        }

        SMS_wmSend(rmsClassArg);

        rmsClassArg.reset_History();
        
        Serial.print("rmsClassArg.get_stateHistoryCount(UWQ)");
        Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
        Serial.print("rmsClassArg.get_totalStateChanges()");
        Serial.println(rmsClassArg.get_totalStateChanges());
    }
    Serial.print("AlarmSituation after update: ");
    Serial.println(rmsClassArg.get_wmAlarmSituation());
    
}

RMSState FSM_decideState(Ezo_board& ezoORPClassArg){
    RMSState state = UWQ;
    if (ezoORPClassArg.get_error() == Ezo_board::SUCCESS){
        state = FSM_implementMLDecision(ezoORPClassArg);

    }
    else {
        state = FWQ;
    }
    return state;
}

RMSState FSM_implementMLDecision(Ezo_board& ezoORPClassArg){
    // TODO: find a better way to easily tune the threshold
    // TODO: allow the ORPValue threshold to be set at initialisation

    RMSState state = UWQ;
    float orpValue = ezoORPClassArg.get_last_received_reading();
    if (orpValue > LOGIT_THRESHOLD) {
        state = SWQ;
    }
    else if (orpValue <= LOGIT_THRESHOLD){
        state = UWQ;
    }

    // TODO: need to deal with faulty reading as well
    return state;
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
