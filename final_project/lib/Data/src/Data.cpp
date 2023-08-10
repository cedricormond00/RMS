#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include <TimeLib.h>

#include "Data.h"

#include "Constant.h"
#include "Global.h"
#include "States.h"

// #include "RTC.h"
#include "Tool.h"
#include "States.h"

bool debug_Data = true;

bool Data_SDCard_init( void ){
    bool success = true;
    
    // Initialize SD card
    SD.begin(SD_CHIPSELECT);

    Serial.print("Initializing SD card...");
    // Set chip select pin as an output
    pinMode(SD_CHIPSELECT, OUTPUT);

    // see if the card is present and can be initialized:
    if (!SD.begin(SD_CHIPSELECT)) {
        Serial.println("Card failed, or not present");
        success = false;
        return success;
    }
    Serial.println("Card initialized.");

    return success;
}


bool Data_createValidDataFileName(char datafilename[]){
    bool success = true;
    char timeStampString[8];
    // uint32_t currentEpochTimeTime = rtc.getEpoch();
    Data_stringTime(rtc.getEpoch(), timeStampString);
    Serial.print("timeStampString: ");
    Serial.println(timeStampString);
    //create the filename string
    // sprintf(datafilename, "%s.csv", timeStampString);
    
    uint16_t i = 0;
    for(i = 0; i< 99; i++){ //24*60*60 = 86400)
        // sprintf(datafilename, "%s_%02d.csv",timeStampString, i);

        sprintf(datafilename, "%s_%01d.csv",timeStampString, i);
        if( !SD.exists(datafilename) ){
            break;      
        }
    }
    if(i == 99){
        datafilename = NULL;
        success = false;
        /* error flag */
    }
    return success;
}


/* convert the ePochTimestamp into a formatted string: ymmdd*/
void Data_stringTime(uint32_t ePochTime, char* buf){
  // Convert the timestamp to a tm structure
  tmElements_t timeInfo;
  breakTime(ePochTime, timeInfo);

  // Format the time components into the provided buffer
  sprintf(buf, "%1d%02d%02d", timeInfo.Year + 1970 - 2020, timeInfo.Month, timeInfo.Day);//, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);
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


bool Data_saveDataPointToDataFile(rmsClass& rmsClassArg,
                                uint8_t inputEventCodeBit,
                                char dataFileName[]){
    // pass as arguments the attributes from the rmsClassArg object
    bool success = Data_saveDataPointToDataFile(rmsClassArg.get_wmReadEPochTime(),
                        rmsClassArg.get_orpReading(),
                        rmsClassArg.get_rmsState(),
                        inputEventCodeBit,
                        rmsClassArg.get_powerStructStablePowerSupply(),
                        rmsClassArg.get_powerStructBatteryVoltage(),
                        rmsClassArg.get_powerStructBatteryELState(),                        
                        rmsClassArg.get_powerStructChargeStatus(),
                        dataFileName);
    return success;
}


bool Data_saveDataPointToDataFile(uint32_t ePochTime,
                                float orpValue,
                                RMSState evaluatedState,
                                uint8_t inputEventCodeBit,
                                bool isPowerSourceStable, 
                                float batteryVoltage,
                                rmsClass::BatteryEnergyLevelState battELState,
                                uint8_t chargeStatus,
                                char dataFileName[]){
    bool success = true;
    // buffer string for readable format time
    char buf[256];
    File dataFile = SD.open(dataFileName, FILE_WRITE);
    
    if (dataFile) {
        Serial.println("writing to SD");

        dataFile.print(ePochTime);
        dataFile.print(",");
        
        Tool_stringTime(ePochTime, buf);
        dataFile.print(buf);
        dataFile.print(",");
        
        if (evaluatedState == FWQ){
            dataFile.print("nan");
        }
        else{
            dataFile.print(orpValue,2);
        }
        dataFile.print(",");

        dataFile.print(evaluatedState);
        dataFile.print(",");
        
        dataFile.print(inputEventCodeBit, BIN);
        dataFile.print(",");

        dataFile.print(triggeredInputEvent, BIN);
        dataFile.print(",");       
        
        dataFile.print(isPowerSourceStable, BIN);
        dataFile.print(",");

        dataFile.print(batteryVoltage, 2);
        dataFile.print(",");   

        dataFile.print(battELState);
        dataFile.print(",");   

        dataFile.print(chargeStatus, BIN);

        dataFile.println();
        
        dataFile.close();
        
    } 
    else {
        success = false;
    }
    dataFile.close();  
    return success;
}


bool Data_updateStateHistory(rmsClass& rmsClassArg, char dataFileName[]){
    bool debug = false;
    bool success = true;
    if (debug && debug_Data){
        Serial.println("before updating counts");
        Serial.print("n_SWQ: ");
        Serial.println(rmsClassArg.get_stateHistoryCount(SWQ));
        Serial.print("n_UWQ: ");
        Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
        Serial.print("n_FWQ: ");
        Serial.println(rmsClassArg.get_stateHistoryCount(FWQ));
        Serial.print("n_meas: ");
        Serial.println(rmsClassArg.get_totalStateChanges());
    }
    

    // Define time frame in milliseconds
    /*
     TODO: if I choose to only use the config file,
     then I should add the cfg as an argument to the function, 
     and directly access the value from there
    */
    uint32_t timeFrame;
    if (Tool_isBitOn(rmsClassArg.get_inputEventCode(), WM_INPUTBIT)){
        timeFrame = rmsClassArg.get_wmAllowedIntervalBetweenSMS();  // Example: 20 sec
    }
    else if (Tool_isBitOn(rmsClassArg.get_inputEventCode(), URA_INPUTBIT)){
        timeFrame = rmsClassArg.get_URAallowedIntervalBetweenSMS();  // Example: 20 sec
    }
    else{
        timeFrame = 600; //default = 10 minutes (10min*60sec/min)
    }
   
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

    Serial.println("");


    while (dataFile.available()) {
        // Read a line from the file
        String line = dataFile.readStringUntil('\n');
        
        // retireve the timestamp
        String timestamp = getValue(line, ',', 0);

        // Parse the timestamp and convert it to an unsigned long
        uint32_t rowTimestamp = timestamp.toInt();

        // Check if the row timestamp falls within the desired time frame
        if ((currentEPochTimestamp - rowTimestamp) <= timeFrame) {
            // Split the line into columns
            String state = getValue(line, ',', 3);

            // Parse the row and convert it to an unsigned long
            RMSState rowState = static_cast<RMSState>(state.toInt());

            //increment the corresponding state (SWQ, UWQ, FWQ) count by 1
            rmsClassArg.set_stateHistoryCount(rowState, rmsClassArg.get_stateHistoryCount(rowState)+1);
            /* 
            - increment the total state changes count
            - update percentage 
            -> these are done immediately upon setting a new count per state
            */

            if (debug && debug_Data){
                Serial.print("Read lines within the timeframe");
                Serial.print("line string");
                Serial.println(line); 
            }
            
        }
    }
    if (debug && debug_Data){
        Serial.println("After updating counts");
        Serial.print("n_SWQ: ");
        Serial.println(rmsClassArg.get_stateHistoryCount(SWQ));
        Serial.print("n_UWQ: ");
        Serial.println(rmsClassArg.get_stateHistoryCount(UWQ));
        Serial.print("n_FWQ: ");
        Serial.println(rmsClassArg.get_stateHistoryCount(FWQ));
        Serial.print("n_meas: ");
        Serial.println(rmsClassArg.get_totalStateChanges());
    }
    
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



// Prints the content of a file to the Serial
void Data_printFile(const char *filename) {
    // Open file for reading
    File file = SD.open(filename);
    if (!file) {
        Serial.println(F("Failed to read file"));
        return;
    }

    // Extract each characters by one by one
    while (file.available()) {
        Serial.print((char)file.read());
    }
    Serial.println();

    // Close the file (File's destructor doesn't close the file)
    file.close();
}