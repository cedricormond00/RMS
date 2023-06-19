#include <Arduino.h>

#include "SMS.h"

#include "Constant.h"
#include "Global.h"

#include "RTC.h"


void SMS_uraSend(rmsClass& rmsClassArg){
    Serial.println("");
    Serial.println("---");
    Serial.print("RMS ");
    Serial.println(RMS_ID);
    Serial.println("URA situation");
    Serial.print("Current State: ");
    Serial.println(rmsClassArg.get_rmsState());
    Serial.print("Last ORP reading: ");
    Serial.println(rmsClassArg.get_orpReading());
    Serial.println("---");
}


void SMS_hbSend(rmsClass& rmsClassArg){
    char buf[40];
    Serial.println("");
    Serial.println("---");
    Serial.print("RMS ");
    Serial.println(RMS_ID);
    Serial.println("HB");
    Serial.print("Current State: ");
    Serial.println(rmsClassArg.get_rmsState());
    Serial.print("Last ORP reading: ");
    Serial.println(rmsClassArg.get_orpReading());
    Serial.print("Current Time: ");
    RTC_getTimeInText(rtc.getEpoch(), buf);
    Serial.println(buf);
    Serial.println("---");
}

void SMS_wmSend(rmsClass& rmsClassArg){
    if (rmsClassArg.get_wmAlarmSituation() == 1){
        SMS_wmImmediate(rmsClassArg);
        rmsClassArg.set_wmAlarmSituation(2);
    }
    else if (rmsClassArg.get_wmAlarmSituation() == 2){
        SMS_wmHistoryWindow(rmsClassArg);
    }
    else if (rmsClassArg.get_wmAlarmSituation() == 3){
        SMS_wmHistoryWindow(rmsClassArg);
        rmsClassArg.set_wmAlarmSituation(0);
    }
    // else if (rmsClassArg.get_wmAlarmSituation() == 1){
    //     SMS_immediateWM(rmsClassArg);
    //     rmsClassArg.set_wmAlarmSituation(2);
    // }
}

void SMS_wmImmediate(rmsClass& rmsClassArg){
    Serial.println("");
    Serial.println("---");
    Serial.print("RMS ");
    Serial.println(RMS_ID);
    Serial.println("WQ situation");
    Serial.print("Current State: ");
    Serial.println(rmsClassArg.get_rmsState());
    Serial.print("Last ORP reading: ");
    Serial.println(rmsClassArg.get_orpReading());
    Serial.println("Update will come in at regular interval until a SWQ is detected at the end of the History Window");
    Serial.println("---");
}

void SMS_wmHistoryWindow(rmsClass& rmsClassArg){
    Serial.println("");
    Serial.println("---");
    Serial.print("RMS ");
    Serial.println(RMS_ID);
    Serial.println("WQ situation");
    Serial.print("Current State: ");
    Serial.println(rmsClassArg.get_rmsState());
    Serial.print("Last ORP reading: ");
    Serial.println(rmsClassArg.get_orpReading());
    Serial.print("SWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(SWQ),2);
    Serial.print("UWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(UWQ),2);
    Serial.print("FWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(FWQ),2);
    Serial.println("---");
}

void SMS_BUPSendIsStablePowerSupply(rmsClass& rmsClassArg){
    if(rmsClassArg.get_smsPowerStructIsStablePowerSupply()){
        /*TODO: could add a further test to ensure that the energy level of the battery is sufficiently high.
        if it is, then send this sms. Ohterwise, dont send the sms, and wait for the next test to send both info at the same time
        */
        Serial.println("---");
        Serial.print("RMS ");
        Serial.println(RMS_ID);
        Serial.println("BUP situation");
        Serial.println("power source switched from mains to external battery");
        Serial.println("---");
    }
    else{
        
        Serial.println("---");
        Serial.print("RMS ");
        Serial.println(RMS_ID);
        Serial.println("BUP situation back to normal");
        Serial.println("power source switched from external battery to mains");
        Serial.println("---");
    }
}

void SMS_BUPSendEnergyLevel(rmsClass& rmsClassArg){

    switch(rmsClassArg.get_powerStructBatteryELState())
    {
    case rmsClass::criticalEL :
        Serial.println("---");
        Serial.print("RMS ");
        Serial.println(RMS_ID);
        Serial.println("Critical Energy Level reached");
        Serial.println("Device will go to deepsleep");
        Serial.println("---");
        break;
    case rmsClass::lowEL :
        Serial.println("---");
        Serial.print("RMS ");
        Serial.println(RMS_ID);
        Serial.println("Low Energy Level reached");
        // Serial.println("Device will soon go to deepsleep");
        Serial.println("---");
        break;
    case rmsClass::sufficientEL :
        Serial.println("---");
        Serial.print("RMS ");
        Serial.println(RMS_ID);
        Serial.println("Sufficient Energy Level reached");
        Serial.println("---");
        break;
    default:
        break;
    }
}
