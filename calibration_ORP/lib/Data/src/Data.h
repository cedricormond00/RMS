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


/**
 * @brief Saves a data point to a data file.
 *
 * This function saves a data point to a data file on the SD card. It takes various parameters representing the data point,
 * such as the attributes from the `rmsClass` object, the input event code bit, and the data file name.
 *
 * @param rmsClassArg The `rmsClass` object containing the data point attributes.
 * @param inputEventCodeBit This tells the operator what input event had triggered the data point monitoring
 * @param dataFileName The name of the data file.
 * @return True if the data point was successfully saved, false otherwise.
 */
bool Data_saveDataPointToDataFile(rmsClass& rmsClassArg,
                                uint8_t inputEventCodeBit,
                                char dataFileName[]);


/**
 * @brief Saves a data point to a data file.
 *
 * This function saves a data point to a data file on the SD card. It takes various parameters representing the data point,
 * such as the epoch time, ORP value, evaluated state, input event code, power source stability, battery voltage, battery
 * energy level state, charge status, and the data file name.
 *
 * @param ePochTime The epoch time of the data point.
 * @param orpValue The ORP value of the data point.
 * @param evaluatedState The state at which the rms is from the reading
 * @param inputEventCodeBit This tells the operator what input event had triggered the data point moitoring
 * @param isPowerSourceStable Whether the power source to the RMS is stable or not
 * @param batteryVoltage The battery voltage when the data point was evaluated
 * @param battELState The battery energy level state when the data point was evaluated
 * @param chargeStatus The charge status when the data point was evaluated
 * @param dataFileName The name of the data file.
 * @return True if the data point was successfully saved, false otherwise.
 */
bool Data_saveDataPointToDataFile(uint32_t ePochTime,
                                float orpValue,
                                RMSState evaluatedState,
                                uint8_t inputEventCodeBit,
                                bool isPowerSourceStable, 
                                float batteryVoltage,
                                rmsClass::BatteryEnergyLevelState battELState,
                                uint8_t chargeStatus,
                                char dataFileName[]);

/**
 * @brief Updates the state history counts (number of each state occurence) for each data points within a specific time frame.
 *
 * This function reads data points from a data file within a specified time frame 
 * and for those qualifying data points, updates the state history 
 * in the `rmsClass` object accordingly (stored inside the stateHistoryStruct). 
 * The function takes the `rmsClass` object and the data file name as parameters.
 *
 * @param rmsClassArg The `rmsClass` object to update the state history counts for.
 * @param dataFileName The name of the data file.
 * @return True if the state history counts were successfully updated, false otherwise.
 */
bool Data_updateStateHistory(rmsClass& rmsClassArg, char dataFileName[]);


String getValue(String data, char separator, int index);

bool Data_saveDataPointToDataFile(rmsClass& rmsClassArg,
                                uint8_t inputEventCodeBit,
                                char dataFileName[]);


    



void Data_printFile(const char *filename);


bool Data_removeFile(char dataFileName[]);






#endif