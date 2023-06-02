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
    Serial.println("");
    Serial.println("---");
    Serial.print("RMS ");
    Serial.println(RMS_ID);
    Serial.println("WQ situation");
    Serial.print("Current State: ");
    Serial.println(rmsClassArg.get_rmsState());
    Serial.print("Last ORP reading: ");
    Serial.println(rmsClassArg.get_orpReading());
    /*TODO: add the percentage of other reading*/
    
    Serial.println("---");
}