#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "Data.h"

#include "Constant.h"
#include "Global.h"
#include "States.h"

#include "RTC.h"

bool Data_saveDataPointToFile(uint32_t ePochTime,
                                float orpValue,
                                RMSState evaluatedState,
                                uint8_t inputEventCodeBit,
                                char dataFileName[]){
   bool success = true;
   char buf[256];
   File dataFile = SD.open(dataFileName, FILE_WRITE);
   
   if (dataFile) {
      RTC_getTimeInText(ePochTime, buf);
      dataFile.print(buf);
      dataFile.print(",");

      dataFile.print(orpValue,DEC);
      dataFile.print(",");

      dataFile.print(evaluatedState);
      dataFile.print(",");

      dataFile.print(inputEventCodeBit);
      dataFile.println();

      dataFile.close();
   } 
   else {
      success = false;
   }
   dataFile.close();  
   return success;
}


bool Data_SDCard_init( void ){
    bool success = true;

    Serial.println(SD_CHIPSELECT);

    // Set chip select pin as an output
    pinMode(SD_CHIPSELECT, OUTPUT);
    
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