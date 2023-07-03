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

void Config_setConfigurationFromFile(ConfigurationStruct& cfg);
void Config_setConfigurationDefault(ConfigurationStruct& cfg);
void Config_saveConfigurationToSD(const ConfigurationStruct& cfg);
void Config_printConfiguration(ConfigurationStruct cfg);


#endif // _CONFIGURATION_H_