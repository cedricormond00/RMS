#include <Arduino.h>

#include "SMS.h"

#include "Constant.h"


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
    Serial.println(rmsClassArg.get_stateHistoryPercentage(SWQ));
    Serial.print("UWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(UWQ));
    Serial.print("FWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(FWQ),2);
    Serial.println("---");
}