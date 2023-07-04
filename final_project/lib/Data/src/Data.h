// Data.h
#ifndef DATA_H // include guard
#define DATA_H

#include <Arduino.h>

#include "States.h"

/**
 * @brief initialises SD card
 * @warning requires to have defined the macro SD_CHIPSELECT
 * @param none
 * @return bool of SD Card initialisation success
*/
bool Data_SDCard_init( void );

/**
 * @brief Creates a valid data file name based on the current timestamp.
 *
 * This function generates a valid data file name by appending a timestamp to the provided datafilename.
 * The timestamp is obtained from the RTC (Real-Time Clock) and formatted as a string.
 * The function iterates through a range of values to ensure uniqueness of the file name and checks if a file with the same name already exists on the SD card.
 * If the maximum number of iterations is reached (99 in this case), indicating an error condition, the function sets datafilename to NULL and returns false.
 *
 * @param datafilename The buffer to store the generated data file name.
 * @return Returns true if a valid data file name is created successfully, or false if an error occurs.
 */
bool Data_createValidDataFileName(char datafilename[]);

/**
 * @brief Converts the given epoch time to a formatted string representation (ymmdd).
 *
 * This function converts the provided epoch time to a formatted string representation and stores it in the given buffer.
 * The function breaks down the epoch time into individual time components (year, month, day) using the tmElements_t structure.
 * The formatted string consists of the year, month, and day components in the specified format: "YMD" (e.g., 00701 for July 1, 2020).
 *
 * @param ePochTime The epoch time value to convert.
 * @param buf The buffer to store the formatted string representation.
 */
void Data_stringTime(uint32_t ePochTime, char* buf);


/**
 * @brief Populates the header row with column names in a file.
 *
 * This function populates the header row of a file with the provided column names.
 * It opens the specified file on the SD card in write mode and writes each column name followed by a comma (',').
 * After writing all the column names, it adds a newline character to move to the next row.
 * Finally, it closes the file.
 *
 * @param colNames An array of column names to be written to the header row.
 * @param numberOfCols The number of columns to be written.
 * @param fileName The name of the file to write the header row.
 * @return Returns true if the header row is populated successfully, or false if an error occurs.
 */
bool Data_populateHeaderRowToFile(String colNames[], uint8_t numberOfCols, char fileName[]);


bool Data_updateStateHistory(rmsClass& rmsClassArg, char dataFileName[]);

String getValue(String data, char separator, int index);

bool Data_saveDataPointToDataFile(rmsClass& rmsClassArg,
                                uint8_t inputEventCodeBit,
                                char dataFileName[]);

bool Data_saveDataPointToDataFile(uint32_t ePochTime,
                                float orpValue,
                                RMSState evaluatedState,
                                uint8_t inputEventCodeBit,
                                bool isPowerSourceStable,
                                float batteryVoltage,
                                rmsClass::BatteryEnergyLevelState battELState,
                                uint8_t chargeStatus,
                                char dataFileName[]);
    



void Data_printFile(const char *filename);


bool Data_removeFile(char dataFileName[]);






#endif