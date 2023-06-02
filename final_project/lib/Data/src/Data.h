// Data.h
#ifndef DATA_H // include guard
#define DATA_H

#include <Arduino.h>

#include "States.h"

bool Data_updateStateHistory(rmsClass& rmsClassArg, char dataFileName[]);

String getValue(String data, char separator, int index);


bool Data_saveDataPointToDataFile(uint32_t ePochTime,
                                float orpValue,
                                RMSState evaluatedState,
                                uint8_t inputEventCodeBit, 
                                char datafilename[]);

bool Data_populateHeaderRowToFile(String rowNames[],
                                uint8_t numberOfRows,
                                char dataFileName[]);        


bool Data_removeFile(char dataFileName[]);


bool Data_SDCard_init( void );



#endif