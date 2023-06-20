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


void Config_setConfigurationDefault(Configuration& cfg){
   cfg.uraPressDuration = 3000;
   cfg.logitThreshold = 450;
   cfg.hbTime = 9;
   cfg.hbElapsePeriod = 24;
   cfg.sendSMS = false;

}


// pass in the default configuration, if a good config can be read from file it will be overwritten 
void Config_setConfigurationFromFile(Configuration& cfg) {
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
        // no config file found, create default configuration 

        Serial.print("No ");
        Serial.print(cfg.filename);
        Serial.println(" found, using default config.");

        Config_setConfigurationDefault(cfg);

        Config_printConfiguration(cfg);
        cfgFile.close();
        return;
    } 
    else {
        // valid cfg file was found, 
        sprintf(buf, "  %s found. \r\n<Configuration: ", cfg.filename);
        Serial.println(buf);

        DynamicJsonDocument doc(1024);

        // Read the content of the configuration file
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
        cfg.hbTime = doc["hbTime"].as<uint8_t>();
        cfg.hbElapsePeriod = doc["hbElapsePeriod"].as<uint8_t>();
        cfg.uraPressDuration = doc["uraPressDuration"].as<uint16_t>();
        cfg.sendSMS = doc["sendSMS"].as<bool>();

        // if (root.containsKey("logitThreshold")) cfg.logitThreshold = root["logitThreshold"];
        // if (root.containsKey("hbTime")) cfg.hbTime = root["hbTime"];
        // if (root.containsKey("hbElapsePeriod")) cfg.hbElapsePeriod = root["hbElapsePeriod"];
        // if (root.containsKey("uraPressDuration")) cfg.uraPressDuration = root["uraPressDuration"];

        Config_printConfiguration(cfg);
    }    
}                      


// Saves the configuration to a file
void Config_saveConfigurationToSD(const Configuration& cfg) {
    // Delete existing file, otherwise the configuration is appended to the file
    SD.remove(cfg.filename);

    // Open file for writing
    File file = SD.open(cfg.filename, FILE_WRITE);
    if (!file) {
        Serial.println(F("Failed to create file"));
        return;
    }

    DynamicJsonDocument doc(1024);

    // Set the values
    doc["logitThreshold"] = cfg.logitThreshold;
    doc["hbTime"] = cfg.hbTime;
    doc["hbElapsePeriod"] = cfg.hbElapsePeriod;
    doc["uraPressDuration"] = cfg.uraPressDuration;
    doc["sendSMS"] = cfg.sendSMS;
    doc["logitThreshold"] = cfg.logitThreshold;

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write configuration to file");
    }

    // Close the file (File's destructor doesn't close the file)
    file.close();
}




void Config_printConfiguration(Configuration& cfg){
    Serial.print("logit threshold (ORP, mV): ");
    Serial.println(cfg.logitThreshold); 

    Serial.print("Heartbeat at: ");
    Serial.print(cfg.hbTime);
    Serial.print("every ");
    Serial.print(cfg.hbElapsePeriod);
    Serial.print("hours");

    Serial.print("User button press required duration");
    Serial.println(cfg.uraPressDuration);
}