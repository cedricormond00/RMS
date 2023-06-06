#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "Data.h"

#include "Constant.h"
#include "Global.h"
#include "States.h"

#include "RTC.h"
#include "States.h"

bool Data_updateStateHistory(rmsClass& rmsClassArg, char dataFileName[]){
    bool success = true;
    Serial.println("before updating counts");
    Serial.print("n_SWQ: ");
    Serial.println(rmsClassArg.get_stateHistoryCount(SWQ));
    Serial.print("n_UWQ: ");
    Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
    Serial.print("n_FWQ: ");
    Serial.println(rmsClassArg.get_stateHistoryCount(FWQ));
    Serial.print("n_meas: ");
    Serial.println(rmsClassArg.get_totalStateChanges());

    // Define time frame in milliseconds
    uint32_t timeFrame = SMS_HW_WQ;  // Example: 20 sec

    // Get current epoch timestamp
    uint32_t currentEPochTimestamp = rmsClassArg.get_wmReadEPochTime();

    File dataFile;

    if (!SD.begin()) {
        // Handle SD card initialization error
        Serial.println("Failed to initialise SD card");
        success = false;
        return success;    
    }

    // Open data file
    dataFile = SD.open(dataFileName);
    if (!dataFile) {
        // Handle file opening error
        Serial.println("Failed to open file");
        success = false;
        return success;
    }

    // Reset the file cursor to the beginning
    dataFile.seek(1);

    // // Start counting
    // int rowCount = 0;
    Serial.println("");


    while (dataFile.available()) {
        // Read a line from the file
        String line = dataFile.readStringUntil('\n');
        
        Serial.print("just reading lines");
        Serial.print("line string");
        Serial.println(line);

        // Split the line into columns
        String timestamp = getValue(line, ',', 0);

        // Parse the timestamp and convert it to an unsigned long
        uint32_t rowTimestamp = timestamp.toInt();

        // Check if the row timestamp falls within the desired time frame
        if ((currentEPochTimestamp - rowTimestamp) <= timeFrame) {
            // Split the line into columns
            String state = getValue(line, ',', 3);

            // Parse the row and convert it to an unsigned long
            RMSState rowState = static_cast<RMSState>(state.toInt());

            rmsClassArg.set_stateHistoryCount(rowState, rmsClassArg.get_stateHistoryCount(rowState)+1);
            
            Serial.print("Read lines within the timeframe");
            Serial.print("line string");
            Serial.println(line); 
        }
    }
    //update percentage -> this is done immediately upon setting a new count per state
    Serial.println("After updating counts");
    Serial.print("n_SWQ: ");
    Serial.println(rmsClassArg.get_stateHistoryCount(SWQ));
    Serial.print("n_UWQ: ");
    Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
    Serial.print("n_FWQ: ");
    Serial.println(rmsClassArg.get_stateHistoryCount(FWQ));
    Serial.print("n_meas: ");
    Serial.println(rmsClassArg.get_totalStateChanges());
    return success;
}

/*helper function to retrieve a certain column from a .csv file*/
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

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
        
        if (evaluatedState == FWQ){
            dataFile.print("NA");
        }
        else{
            dataFile.print(evaluatedState);
        }
        dataFile.print(",");
        
        dataFile.print(inputEventCodeBit, BIN);

        dataFile.print(",");
        dataFile.print(triggeredInputEvent, BIN);

        


        dataFile.println();
        
        dataFile.close();
        
    } 
    else {
        success = false;
    }
    dataFile.close();  
    return success;
}

bool Data_populateHeaderRowToFile(String colNames[],
                                uint8_t numberOfCols,
                                char fileName[]){
    bool success = true;
    
    File file = SD.open(fileName, FILE_WRITE);
    if (file) {
        for (int i=0; i<numberOfCols; i++){
            file.print(colNames[i]);
            if (i<numberOfCols-1){
                file.print(",");
            }
            
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