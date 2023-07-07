//FSM.h
// functions related to FSM 

#ifndef FSM_H
#define FSM_H

// #include <Arduino.h>
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib

#include "States.h"
#include "Configuration.h"


/**
 * @brief Initializes the RMS (Remote Monitoring System) with the provided configuration.
 *
 * This function initializes the RMS by setting the required values from the configuration file into the `rmsClass` object.
 * It sets the sleep periods, allowed intervals between SMS, input event code, wake-up epoch times, power situation,
 * SMS power structure, and state history of the RMS. It also ensures that the heartbeat alarm is set in the future.
 *
 * @param rmsClassArg The RMS class object to be initialized.
 * @param cfgStructArg The configuration struct containing the required values.
 */
void FSM_initRMS(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

void FSM_updateInputEventCode(rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, volatile uint8_t* triggeredInputEvent);//input arguments: relevant function counter


/**
 * @brief Executes the appropriate function based on the input event code.
 *
 * @param EZO_ORP The Ezo_board object for ORP measurement.
 * @param rmsClassArg The rmsClass object containing system information.
 * @param rtcClassArg The RTCZero object for real-time clock operations.
 * @param cfgStructArg The configuration settings for the system.
 * @param dataFileName The name of the data file.
 * @return void
 *
 * @details  * This function is responsible for executing the appropriate function based on the input event code.
 * It takes the necessary parameters for function execution and performs the following steps:
 - Find the function to execute based on the input event code obtained from rmsClassArg.
 - Execute the water monitoring function (FSM_f_WM_EZO) if the WM (Water Monitoring) input event bit is set.
 - Execute the user response analysis function (FSM_f_URA) if the URA (User Response Analysis) input event bit is set.
 - Execute the heartbeat function (FSM_f_HB) if the HB (Heartbeat) input event bit is set.
 - Execute the backup power function (FSM_f_BUP) if the BUP input event bit is set.
 */
void FSM_executeFunction(Ezo_board& EZO_ORP, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]);


/**
 * @brief Water monitoring execution function
 *
 *  1. Get current time from RTC.
    2. Store current epoch time, as the water monitoring time.
    3. Read ORP value from EZO board.
    4. Store ORP value in rmsClassArg object.
    5. Evaluate RMS state based on ORP value and config settings.
    6. Update rmsClassArg with new state.
    7. Determine sleep period for the device.
    8. Calculate next wake-up time.
    9. Check battery health and power situation.
    10. Save data point to data file.
    11. Print data write success status.
    12. Perform multiple alarm management.
 * It takes the `Ezo_board`, `rmsClass`, `RTCZero`, `ConfigurationStruct`, and data file name as parameters.
 *
 * @param ezoClassArg The `Ezo_board` object representing the EZO board.
 * @param rmsClassArg The `rmsClass` object containing the water monitoring system information.
 * @param rtcClassArg The `RTCZero` object representing the real-time clock.
 * @param cfgStructArg The `ConfigurationStruct` object containing the configuration settings.
 * @param dataFileName The name of the data file to which data points will be saved.
 */
void FSM_f_WM_EZO(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg, ConfigurationStruct cfgStructArg, char dataFileName[]);

/**
 * @brief User Raised Alarm (URA) Execution function
 * 
 * @param ezoClassArg The Ezo_board object representing the ORP sensor.
 * @param rmsClassArg The rmsClass object containing the RMS state and data.
 * @param rtcClassArg The RTCZero object for accessing the current time.
 * @param cfgStructArg The configuration settings for the system.
 * @param dataFileName The name of the data file for storing the data points.
 * 
 * @return None.
 * 
 * @details This function performs the URA function, which involves 
 * 1- reading the ORP value,
 * 2- deciding on the RMS state based on the ORP value and configuration settings,
 * 3- updating the power situation, 
 * 4- storing the data point, 
 * 5- sending an SMS if necessary.
 * The URA process is executed periodically based on the user input.
 * 
 * @note The function assumes that the necessary objects and libraries have been properly
 * initialized and set up prior to calling this function.
 * 
 * @warning The function relies on the correct configuration settings and proper functioning
 * of the ORP sensor, RTC, data storage, and SMS functionality. Ensure that these components
 * are set up correctly to ensure the desired behavior of the URA process.
 */
void FSM_f_URA(Ezo_board& ezoClassArg, rmsClass& rmsClassArg, RTCZero& rtcClassArg,
    ConfigurationStruct cfgStructArg, char dataFileName[]);


/**
 * @brief Performs Heartbeat (HB) operations and sends the HB SMS message.
 *
 * @param rmsClassArg The rmsClass object containing system information.
 * @param cfgStructArg The configuration settings for the system.
 * @return void
 *
 * @details  This function is responsible for performing HB operations and sending an SMS message to indicate
 * a heartbeat event. It updates the HB epoch time using the RTC_updateHBEPochTime() function, which
 * calculates the next heartbeat time based on the current time and the HB interval specified in the
 * configuration settings. It then sends the HB SMS message using the SMS_hbSend() function.
 *
 * @note The HB SMS message is sent only if the HB feature is enabled. 
 * The LED toggling is performed only if the debug mode is enabled (debug = true).
 */
void FSM_f_HB(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);


/**
 * @brief Performs Battery Unplugged/Plugged (BUP) operations and sends relevant SMS messages.
 *
 * @param rmsClassArg The rmsClass object containing system information.
 * @param cfgStructArg The configuration settings for the system.
 * @return void
 *
 * @details  This function is responsible for performing BUP operations and sending SMS messages to inform
 * about the stability of the power supply and the energy level of the battery. It checks for changes
 * in the power stability situation and sends an SMS if the stability status has changed. It also sends
 * an SMS when the energy level of the battery reaches a critical or low level. Additionally, if the
 * device is running on battery power and the energy level reaches a critical level, the function puts
 * the device to sleep mode.
 *
 * @note This function assumes that the necessary SMS flags and state variables in the rmsClass object
 * are correctly updated and reflect the current power stability situation and battery energy level state.
 * It relies on the SMS_BUPSendIsStablePowerSupply() and SMS_BUPSendEnergyLevel() functions to send the
 * appropriate SMS messages.
 *
 * @warning The BUP SMS messages are sent only if the BUP feature is enabled
 */
void FSM_f_BUP(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Determines the RMS state based on ORP readings and configuration settings.
 *
 * This function takes an Ezo_board object and a ConfigurationStruct object as input parameters
 * and uses the ORP readings and configuration settings to determine the RMS state.
 *
 * @param ezoORPClassArg An Ezo_board object representing the ORP sensor.
 * @param cfgStructArg A ConfigurationStruct object containing the configuration settings.
 *
 * @return The RMSState representing the determined state:
 *         - SWQ
 *         - UWQ 
 *         - FWQ
 */
RMSState FSM_decideState(Ezo_board& ezoORPClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Implements the machine learning decision algorithm to determine the RMS state based on ORP readings and configuration settings.
 *
 * This function takes an Ezo_board object and a ConfigurationStruct object as input parameters
 * and uses the ORP readings and configuration settings to implement a machine learning decision algorithm
 * in order to determine the RMS state.
 *
 * @param ezoORPClassArg An Ezo_board object representing the ORP sensor.
 * @param cfgStructArg A ConfigurationStruct object containing the configuration settings.
 *
 * @return The RMSState representing the determined state:
 *         - SWQ (Safe Water Quality) if the ORP reading is above the configured threshold value.
 *         - UWQ (Unsafe Water Quality) if the ORP reading is below or equal to the configured threshold value.
 */
RMSState FSM_implementMLDecision(Ezo_board& ezoORPClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Updates the power situation in the RMS class object based on battery health information.
 *
 * This function retrieves the battery health information using Battery functions and updates
 * the power-related variables in the rmsClass object. It sets the battery voltage, stable power supply status,
 * and charge status in the power structure of the rmsClass object.
 *
 * @param rmsClassArg The rmsClass object to update with the power situation information.
 */
void FSM_setPowerSituation(rmsClass& rmsClassArg);

/**
 * @brief Manages alarm situations and sends SMS notifications based on the water monitoring system's state and alarm conditions.
 *
 * This function checks the current state of the water monitoring system and the alarm situation in the `rmsClass` object.
 * It determines if an SMS notification needs to be sent based on different conditions such as detecting an anomaly (UWQ or FWQ)
 * or reaching the end of the history window duration. It updates the alarm situation (inside the RMS class object, (stateHistoryStruct)), 
 * sends SMS notifications when necessary, and performs necessary data updates and resets. 
 * The function relies on various configuration settings and the current time.
 *
 * @param rmsClassArg The rmsClass object representing the water monitoring system.
 * @param cfgStructArg The configuration settings for the water monitoring system.
 * @param currentTime The current time in epoch format.
 * @param dataFileName The name of the data file containgin the data.
 */
void FSM_multipleAlarmManagement(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg, uint32_t currentTime, char dataFileName[]);


#endif