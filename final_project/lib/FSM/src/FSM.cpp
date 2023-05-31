#include <Arduino.h>
#include <ArduinoLowPower.h>


#include <iot_cmd.h>
#include <Ezo_i2c_util.h>  

#include "FSM.h"
#include "Constant.h"
#include "Global.h"
#include "LED.h"
#include "I2c.h"
#include "Tool.h"

bool debugDisplay = 1;


void FSM_executeFunction(Ezo_board* EZO_ORP, rmsClass& rmsClassArg, RTCZero& rtcClassArg){
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

        FSM_f_URA();
        Tool_setBitOff(&eventInputCode, URA_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        
        if (debug){

            Serial.print("event Input Code after update: ");
            Serial.print(eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(eventInputCode);
        }
    }

    // if (*eventInputCode_ & BUP_INPUTBIT){
    //     // Serial.println("in if");
    //     if (debug){
    //         Serial.println("in FSM_executeFunction, BUP");
    //         Serial.print("event Input Code before update: ");
    //         Serial.print(*eventInputCode_, BIN);
    //         Serial.print(", ");
    //         Serial.println(*eventInputCode_);
    //     }
    //     // FSM_BUPMONITORINGFUNCTION;
    //     Tool_setBitOff(eventInputCode_, BUP_INPUTBIT); // because eventInputCode_ is already the address of the pointer
    //                                                 // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
    //     if (debug){
    //         Serial.print("event Input Code after update: ");
    //         Serial.print(*eventInputCode_, BIN);
    //         Serial.print(", ");
    //         Serial.println(*eventInputCode_);
    //     }
    // }
    rmsClassArg.set_inputEventCode(eventInputCode);
    if (debug){
        // Serial.println("");
    }
}

void FSM_f_WM_EZO(Ezo_board* classArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg){
    rmsClassArg.set_wmReadEPochTime(rtcClassArg.getEpoch());
    Serial.print("rmsClassArg.get_wmReadEPochTime(): ");
    Serial.println(rmsClassArg.get_wmReadEPochTime());
    FSM_getEzoWaterReading(classArg);
    
    
    // classArg->send_read_cmd();
    // delay(815); // delay required for reading command
    // // TODO: eventually, will need to create my own fuction that only reads the values and that can then be used to store
    // receive_and_print_reading(*classArg);

    float ORPValue = classArg->get_last_received_reading();
    // TODO: allow the ORPValue threshold to be set at initialisation
    if (ORPValue > 400) {
        rmsClassArg.set_rmsState(SWQ);
    }
    else 
    // TODO: need to deal with faulty reading as well
    {
        rmsClassArg.set_rmsState(UWQ);
    }
    // rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wakeUpEPochTime()+rmsClassArg.get_sleepPeriod());
    // This solution would allow to minisme this drift. However, it would probably require to add a check to ensure the next wakup alarm time is later then the present time
    // TODO: add a check to ensure the next wakeup alarm is later then the present time
    rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wmWakeUpEPochTime()+rmsClassArg.get_sleepPeriod());
    
    // rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_wmReadEPochTime()+rmsClassArg.get_sleepPeriod());

    // should be added literaly just before going to sleep
    rtcClassArg.setAlarmEpoch(rmsClassArg.get_nextWakeUpEPochTime());
}


void FSM_getEzoWaterReading(Ezo_board* classArg){
    classArg->send_read_cmd();
    delay(815); // delay required for reading command
    // TODO: eventually, will need to create my own fuction that only reads the values and that can then be used to store
    receive_and_print_reading(*classArg);
}


void FSM_f_URA(){
    ToggleLED(ORANGELED_PIN);
    debugDisplay = 1;

}


void FSM_updateInputEventCode(rmsClass& rmsClassArg, RTCZero& rtcClassArg, volatile uint8_t* triggeredInputEvent)//input arguments: relevant function counter
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
        
        //if include 9AM alarm -> add the check to see if we are just close to 9AM
        // double gate: one for the time window, and one for a boolean to inform if the HB's been sent. Once out ot the time window, reset to the boolean
        Serial.print("Current unix time: ");
        Serial.println(currentTime);
        Serial.print("rmsClassArg.get_wakeUpEPochTime(): ");
        Serial.println(rmsClassArg.get_wakeUpEPochTime());
        Serial.print("rmsClassArg.get_wmWakeUpEPochTime(): ");
        Serial.println(rmsClassArg.get_wmWakeUpEPochTime());

        // Check to see if the alarm match occured at a time when the device was not asleep
        // alarm occured when device not asleep 
        // -> artificially use current time
        // if (currentTime-rmsClassArg.get_wmWakeUpEPochTime()<=rmsClassArg.get_sleepPeriod()){ //alarm occured when the device was not asleep
        if (Tool_isBitOn(*triggeredInputEvent, 0b11111110) || Tool_isBitOn(rmsClassArg.get_inputEventCode(), 0b11111110)){ //alarm occured when the device was not asleep
            rmsClassArg.set_wmWakeUpEPochTime(currentTime); //+1

            Serial.print("Alarm match occured when device awake: ");
            Serial.print("rmsClassArg.get_wmWakeUpEPochTime(): ");
            Serial.println(rmsClassArg.get_wmWakeUpEPochTime());
        }
        // alarm occured when the device was asleep
        // -> use the time at which the device wokeup
        else{ 

            rmsClassArg.set_wmWakeUpEPochTime(rmsClassArg.get_wakeUpEPochTime());
            Serial.print("Alarm match occured when device asleep: ");
            Serial.print("rmsClassArg.get_wmWakeUpEPochTime(): ");
            Serial.println(rmsClassArg.get_wmWakeUpEPochTime());
        }
        // in case the alarm match occured at some other time then when waking up from sleep
        // if (rmsClassArg.get_nextWakeUpEPochTime() <= currentTime){
        //     Serial.println("came in if time test");
        //     rmsClassArg.set_wmWakeUpEPochTime(currentTime);
        // }

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
