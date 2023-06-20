/*! \Configuration.h
 *  \Configuration file utilities 
 */
#ifndef _CONFIGURATION_H_
   #define _CONFIGURATION_H_

#define NBR_JSON_OBJECTS_IN_CFG 26



struct Configuration {
    // eventually, could make this hard coded: instead input the MS target etc and arduino finds optimal parameters
    uint16_t logitThreshold = 450; // decision boundary limit for the ML model
    uint8_t hbTime = 9; // define at what time we want a HB
    uint8_t hbElapsePeriod = 24; // duration (in hours) between 2 HB alarms
    uint16_t uraPressDuration = 3000; //required user button press duration. in ms.
    bool sendSMS = false;
    /*TODO: 
    - add the threshold for battery level decision?
    - add the sleep duration for all state
    */
    char filename[20] = "RMS_V1.CFG";

};

void Config_setConfigurationFromFile(Configuration& cfg);
void Config_setConfigurationDefault(Configuration& cfg);
void Config_saveConfigurationToSD(const Configuration& cfg);
void Config_printConfiguration(Configuration& cfg);


#endif // _CONFIGURATION_H_