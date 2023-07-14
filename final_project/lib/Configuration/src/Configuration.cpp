/*! \Configuration.cpp
 *  \Configuration file utilities 
 */




#include <Arduino.h>
// #include <iostream.h>

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>      //https://github.com/bblanchon/ArduinoJson/issues/223, https://github.com/bblanchon/ArduinoJson

#include "Configuration.h"

#include "Data.h"


void Config_setConfigurationDefault(ConfigurationStruct& cfg){
    cfg.logitThreshold = 450;

    cfg.uraPressDuration = 3000; //required user button press duration. in ms.
    
    cfg.hbTargetHour = 9; // Hour at which the first hb muist occur
    cfg.hbIntervalHour = 24 ; // interval desired between each hb
    
    cfg.swqSleepPeriod = 10;
    cfg.uwqSleepPeriod = 10;
    cfg.fwqSleepPeriod = 10;
    
    cfg.uraSMSInterval = 20;
    cfg.wmSMSInterval = 20;

    cfg.sendSMS = false;


    char remoteNumber[] = {'0','0','4','1','7','6','7','2','4','7','4','4','9','\0'};
    strcpy(cfg.remoteNumber, remoteNumber);
    cfg.remoteNumberLength = strlen(remoteNumber);
}


// pass in the default configuration, if a good config can be read from file it will be overwritten 
void Config_setConfigurationFromFile(ConfigurationStruct& cfg) {
    char buf[128];
    // char json[1024];

    // Open the SD card
    if (!SD.begin()) {
        Serial.println("Error initializing SD card");
        return;
    }

    // open the configuration file
    File cfgFile = SD.open(cfg.filename, FILE_READ);
    if( cfgFile == 0 ) {
        // no config file found, use default configuration settings

        Serial.print("No ");
        Serial.print(cfg.filename);
        Serial.println(" found, using default config:");

        Config_setConfigurationDefault(cfg);
        cfgFile.close();
        return;
    } 
    else {
        // valid cfg file was found, use the configuration settings from the file
        sprintf(buf, "  %s found. \r\n<Configuration: ", cfg.filename);
        Serial.println(buf);

        //TODO: sort out how to find the right size
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, cfgFile);
        cfgFile.close();

        if (error) {
            Serial.println("Error parsing configuration file");
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        }

        // Extract the values from the JSON document
        cfg.logitThreshold = doc["logitThreshold"].as<uint16_t>();

        cfg.uraPressDuration = doc["uraPressDuration"].as<uint16_t>();
        
        cfg.hbTargetHour = doc["hbTargetHour"].as<uint8_t>();
        cfg.hbIntervalHour = doc["hbIntervalHour"].as<uint8_t>();
        
        cfg.swqSleepPeriod = doc["swqSleepPeriod"].as<uint32_t>();
        cfg.uwqSleepPeriod = doc["uwqSleepPeriod"].as<uint32_t>();
        cfg.fwqSleepPeriod = doc["fwqSleepPeriod"].as<uint32_t>();
        
        cfg.uraSMSInterval = doc["uraSMSInterval"].as<uint32_t>();
        cfg.wmSMSInterval = doc["wmSMSInterval"].as<uint32_t>();
        
        cfg.sendSMS = doc["sendSMS"].as<bool>();
        

        cfg.remoteNumberLength = doc["remoteNumberLength"].as<uint8_t>();

        const char* remoteNumber = doc["remoteNumber"];
        Serial.print("remoteNumber: ");
        Serial.println(remoteNumber);
        strcpy(cfg.remoteNumber, remoteNumber);
    }    
}                      


void Config_printConfiguration(ConfigurationStruct cfg){
    Serial.print("logit threshold (ORP, mV): ");
    Serial.println(cfg.logitThreshold); 

    Serial.print("Heartbeat at: ");
    Serial.print(cfg.hbTargetHour);
    Serial.print(", every: ");
    Serial.print(cfg.hbIntervalHour);
    Serial.println(" hours.");

    Serial.print("User button press required duration (in ms): ");
    Serial.println(cfg.uraPressDuration);

    Serial.println("Sleep period (in sec):");
    Serial.print("swq: ");
    Serial.println(cfg.swqSleepPeriod);
    Serial.print("uwq: ");
    Serial.println(cfg.uwqSleepPeriod);
    Serial.print("fwq: ");
    Serial.println(cfg.fwqSleepPeriod);

    Serial.println("Interval between SMS:");
    Serial.print("URA: ");
    Serial.println(cfg.uraSMSInterval);
    Serial.print("WM: ");
    Serial.println(cfg.wmSMSInterval);

    Serial.print("Request to send SMS: ");
    Serial.println(cfg.sendSMS);
    Serial.print("Sending SMS to: ");
    Serial.println(cfg.remoteNumber);
    Serial.print("Phone Number length: ");
    Serial.println(cfg.remoteNumberLength);
}


// Saves the configuration to a file
void Config_saveConfigurationToSD(const ConfigurationStruct& cfg) {
    // Delete existing file, Otherwise the configuration is appended to the file
    SD.remove(cfg.filename);

    // Open file for writing
    File file = SD.open(cfg.filename, FILE_WRITE);
    if (!file) {
        Serial.println(F("Failed to create file"));
        return;
    }

    //TODO: sort out the lenght required
    DynamicJsonDocument doc(2048);

    // Set the values
    doc["logitThreshold"] = cfg.logitThreshold;

    doc["uraPressDuration"] = cfg.uraPressDuration;
    
    doc["hbTargetHour"] = cfg.hbTargetHour;
    doc["hbIntervalHour"] = cfg.hbIntervalHour;

    doc["swqSleepPeriod"] = cfg.swqSleepPeriod;
    doc["uwqSleepPeriod"] = cfg.uwqSleepPeriod;
    doc["fwqSleepPeriod"] = cfg.fwqSleepPeriod;
    
    doc["uraSMSInterval"] = cfg.uraSMSInterval;
    doc["wmSMSInterval"] = cfg.wmSMSInterval;

    doc["sendSMS"] = cfg.sendSMS;
    doc["remoteNumberLength"] = cfg.remoteNumberLength;
    doc["remoteNumber"] = cfg.remoteNumber;

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write configuration to file");
    }

    // Close the file (File's destructor doesn't close the file)
    file.close();
}


bool Config_checkSMS(ConfigurationStruct& cfgStructArg){
    bool success = true;
    if (cfgStructArg.sendSMS){
        if (strlen(cfgStructArg.remoteNumber) == cfgStructArg.remoteNumberLength
        && cfgStructArg.remoteNumberLength!=0){
            return success;
        }
        else{
            success = false;
            return success;
        }
    }
    return success;
}