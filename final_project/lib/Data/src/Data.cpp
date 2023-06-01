#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "Data.h"

#include "Constant.h"
#include "Global.h"
#include "States.h"

#include "RTC.h"

bool Data_saveDataPointToDataFile(uint32_t ePochTime,
                                float orpValue,
                                RMSState evaluatedState,
                                uint8_t inputEventCodeBit,
                                char dataFileName[]){
    bool success = true;
    char buf[256];
    File dataFile = SD.open(dataFileName, FILE_WRITE);
    
    if (dataFile) {
        dataFile.print(ePochTime);
        dataFile.print(",");
        
        RTC_getTimeInText(ePochTime, buf);
        dataFile.print(buf);
        dataFile.print(",");
        
        dataFile.print(orpValue,2);
        dataFile.print(",");
        
        dataFile.print(evaluatedState);
        dataFile.print(",");
        
        dataFile.print(inputEventCodeBit, BIN);
        dataFile.println();
        
        dataFile.close();
        
    } 
    else {
        success = false;
    }
    dataFile.close();  
    return success;
}

bool Data_populateHeaderRowToFile(String rowNames[],
                                uint8_t numberOfRows,
                                char fileName[]){
    bool success = true;
    
    File file = SD.open(fileName, FILE_WRITE);
    if (file) {
        for (int i=0; i<numberOfRows; i++){
            file.print(rowNames[i]);
            file.print(",");
        }
        file.println();
        file.close();
    } 
    else {
        success = false;
    }
    file.close();  
    return success;
}

bool Data_removeFile(char fileName[]){
    bool success = true;

    if (SD.exists(fileName)) {
        Serial.print(fileName);
        Serial.println(" exists.");
    }
    else {
        Serial.print(fileName);
        Serial.println(" doesn't exist.");
    }
    // delete the file:
    Serial.print("Removing ");
    Serial.println(fileName);
    
    SD.remove(fileName);
    
    Serial.print(fileName);
    Serial.println(" Removed");

    if (SD.exists(fileName)) {
        success = false;
    } 
    return success;
}

bool Data_SDCard_init( void ){
    bool success = true;

    // // Set chip select pin as an output
    // pinMode(SD_CHIPSELECT, OUTPUT);
    
    // Initialize SD card
    SD.begin(SD_CHIPSELECT);

    Serial.print("Initializing SD card...");
    pinMode(SD_CHIPSELECT, OUTPUT);

    // see if the card is present and can be initialized:
    if (!SD.begin(SD_CHIPSELECT)) {
        Serial.println("Card failed, or not present");
        success = false;
        return success;
    }
    Serial.println("card initialized.");

    return success;
}