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


void setDefaultConfiguration(Configuration& cfg){
   cfg.uraPressDuration = 3000;
   cfg.logitThreshold = 450;
   cfg.hbTime = 9;
   cfg.hbElapsePeriod = 24;
}


// pass in the default configuration, if a good config can be read from file it will be overwritten 
void Config_setConfigurationFromFile(Configuration& cfg) {
    char buf[128];
    char json[1024];

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

        setDefaultConfiguration(cfg);

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
            return;
        }

        if (root.containsKey("logitThreshold")) cfg.logitThreshold = root["logitThreshold"];
        if (root.containsKey("hbTime")) cfg.hbTime = root["hbTime"];
        if (root.containsKey("hbElapsePeriod")) cfg.hbElapsePeriod = root["hbElapsePeriod"];
        if (root.containsKey("uraPressDuration")) cfg.uraPressDuration = root["uraPressDuration"];
        
        Config_printConfiguration(cfg);
    }    
}                      


// Saves the configuration to a file
void saveConfiguration(const Configuration& cfg) {
    // Delete existing file, otherwise the configuration is appended to the file
    SD.remove(cfg.filename);

    // Open file for writing
    File file = SD.open(cfg.filename, FILE_WRITE);
    if (!file) {
        Serial.println(F("Failed to create file"));
        return;
    }

    // Allocate the memory pool on the stack
    // Don't forget to change the capacity to match your JSON document.
    // Use https://arduinojson.org/assistant/ to compute the capacity.
    StaticJsonBuffer<512> jsonBuffer;

    // Parse the root object
    JsonObject &root = jsonBuffer.createObject();

    // Set the values
    root["logitThreshold"] = cfg.logitThreshold;
    root["hbTime"] = cfg.hbTime;
    root["hbElapsePeriod"] = cfg.hbElapsePeriod;
    root["uraPressDuration"] =  cfg.uraPressDuration;

    // Serialize JSON to file
    if (root.printTo(file) == 0) {
        Serial.println(F("Failed to write to file"));
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