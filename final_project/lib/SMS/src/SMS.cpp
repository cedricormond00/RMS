#include <Arduino.h>

#include "Constant.h"
#include "States.h"

void SMS_sendURA(rmsClass& rmsClassArg){
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

void SMS_sendWM(rmsClass& rmsClassArg){
    char buf[40];
    Serial.println("");
    Serial.println("---");
    Serial.print("RMS ");
    Serial.println(RMS_ID);
    Serial.println("WQ situation");
    Serial.print("Current State: ");
    Serial.println(rmsClassArg.get_rmsState());
    Serial.print("Last ORP reading: ");
    Serial.println(rmsClassArg.get_orpReading());
    // sprintf(buf, "SWQ percentage %f", rmsClassArg.get_stateHistoryPercentage(SWQ));
    Serial.print("SWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(SWQ),2);
    Serial.print("UWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(UWQ),2);
    Serial.print("FWQ percentage");
    Serial.println(rmsClassArg.get_stateHistoryPercentage(FWQ),2);
    /*TODO: add the percentage of other reading*/
    
    Serial.println("---");
}