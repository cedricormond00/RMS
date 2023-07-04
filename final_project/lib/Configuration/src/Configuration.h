/*! \Configuration.h
 *  \Configuration file utilities 
 */
#ifndef _CONFIGURATION_H_
   #define _CONFIGURATION_H_

#define NBR_JSON_OBJECTS_IN_CFG 26



struct ConfigurationStruct {
    // eventually, could make this hard coded: instead input the MS target etc and arduino finds optimal parameters
    // pass cfg as argument
    uint16_t logitThreshold; // decision boundary limit for the ML model
    // uint8_t hbTime = 9; // define at what time we want a HB
    // uint8_t hbElapsePeriod = 24; // duration (in hours) between 2 HB alarms
    //TODO: decide if I want to use the setuo where I store these values inside my rmsClass, or if I directly access them from the COnfiguration
    // pass cfg as argument
    uint16_t uraPressDuration; //required user button press duration. in ms.
    // pass cfg as argument
    uint8_t hbTargetHour; // Hour at which the first hb muist occur
    // pass cfg as argument
    uint8_t hbIntervalHour; // interval desired between each hb
    // use an initialisation function in the INIT state
    uint32_t swqSleepPeriod;
    // use an initialisation function in the INIT state
    uint32_t uwqSleepPeriod;
    // use an initialisation function in the INIT state
    uint32_t fwqSleepPeriod;
    // use an initialisation function in the INIT state
    uint32_t uraSMSInterval;
    // use an initialisation function in the INIT state
    uint32_t wmSMSInterval;
    // set this up in the setup function: call SMS function
    bool sendSMS = false;

    uint8_t remoteNumberLength;
    char remoteNumber[20];

    /*TODO: 
    - add the threshold for battery level decision?
    - add the sleep duration for all state
    */
    char filename[20] = "RMS_V1.CFG";

};

//TODO: set values to final desired values
/**
 * @brief Sets the default configuration values in the provided ConfigurationStruct.
 *
 * This function sets the default values for various configuration parameters in the provided ConfigurationStruct object.
 * It assigns default values to the following parameters:
 * - logitThreshold: 450
 * - uraPressDuration: 3000 (required user button press duration in milliseconds)
 * - hbTargetHour: 9 (hour at which the first heartbeat must occur)
 * - hbIntervalHour: 24 (desired interval between each heartbeat)
 * - swqSleepPeriod: 10 (sleep period for swq)
 * - uwqSleepPeriod: 10 (sleep period for uwq)
 * - fwqSleepPeriod: 10 (sleep period for fwq)
 * - uraSMSInterval: 20 (URA SMS interval)
 * - wmSMSInterval: 20 (WM SMS interval)
 * - sendSMS: false (flag indicating whether to send SMS)
 * - remoteNumber: "0041767247449" (default remote phone number)
 *
 * @param cfg The ConfigurationStruct object to be updated with default values.
 */
void Config_setConfigurationDefault(ConfigurationStruct& cfg);


/**
 * @brief Sets the configuration values from a file to the provided ConfigurationStruct.
 *
 * This function reads the configuration values from the specified file on the SD card and updates the provided ConfigurationStruct object with the read values.
 * If the configuration file is not found, it uses the default configuration values by calling `Config_setConfigurationDefault`.
 * The function opens the configuration file, parses the JSON content using the ArduinoJson library, and extracts the values into the ConfigurationStruct object.
 * If an error occurs during file operations or JSON parsing, an error message is printed.
 * After extracting the values, it calls `Config_printConfiguration` to display the updated configuration.
 *
 * @param cfg The ConfigurationStruct object to be updated with the configuration values from the file.
 */
void Config_setConfigurationFromFile(ConfigurationStruct& cfg);


/**
 * @brief Prints the configuration values to the serial monitor.
 *
 * @param cfg The configuration structure containing the values to print.
 */
void Config_printConfiguration(ConfigurationStruct cfg);

/**
 * @brief Saves the configuration to a file on the SD card.
 *
 * This function saves the provided configuration structure to a file on the SD card.
 * It first removes any existing file with the same name to ensure a clean write.
 * The configuration values are serialized into a JSON document and written to the file.
 *
 * @param cfg The configuration structure to be saved.
 */
void Config_saveConfigurationToSD(const ConfigurationStruct& cfg);


#endif // _CONFIGURATION_H_