//SMS.h
#ifndef SMS_H
#define SMS_H

#include "States.h"
#include "Configuration.h"

/**
 * @brief function to initialisze nb object 
 * @details sets 
 * - the timeout 
*/
void SMS_init(uint32_t timeout);


/**
 * @brief Sends an SMS message using the configured SMS settings.
 *
 * @param cfgStructArg    The configuration structure containing the SMS settings.
 * @param message         The message content to be sent as an SMS.
 *
 * @details This function checks if sending SMS is enabled in the configuration settings (`cfgStructArg.sendSMS`).
 * If enabled, it initializes the SMS connection and sends the specified `message` to the remote number
 * specified in the configuration settings (`cfgStructArg.remoteNumber`).
 *
 * @warning The function assumes that the necessary libraries and dependencies are included,
 * and the SMS_initConnection and SMS_sendMessage functions are defined separately.
 * Please ensure that the SMS functionality is properly configured and initialized before calling this function.
 */
void SMS_sendSMS(ConfigurationStruct cfgStructArg, char message[160]);

/**
 * @brief Initializes the network connection for sending SMS.
 *
 * @return Boolean value indicating the connection status.
 *
 * @details This function attempts to establish the connection for sending SMS by checking the network status.
 * It makes multiple attempts (up to 5 times) to connect to the network using NB-IoT.
 * If the network status becomes NB_READY, the connection is considered initialized and the function returns true.
 * If the connection fails after the attempts, the function returns false.
 *
 * @warning The function assumes that the necessary libraries and dependencies are included,
 * and the nbAccess object for NB-IoT communication is properly set up and initialized before calling this function.
 */
bool SMS_initConnection();


/**
 * @brief Sends SMS notifications based on the current alarm situation.
 *
 * 
 * @param rmsClassArg The `rmsClass` object containing the water monitoring system information.
 * @param cfgStructArg The `ConfigurationStruct` object containing the configuration settings.
 * 
 * @details This function tells which SMS notifications to send, based on the current alarm situation in the water monitoring system,
 * as determined by the `rmsClass` object and the configuration settings in the `ConfigurationStruct` object.
 * It takes the `rmsClass` object and the `ConfigurationStruct` object as parameters.
 *
 */
void SMS_wmSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Sends an SMS message to the specified remote number.
 *
 * @param message The message to be sent (up to 160 characters).
 * @param remoteNumber The phone number of the recipient.
 * @return Boolean value indicating the success of sending the message.
 *
 * @details This function sends an SMS message to the specified remote number.
 * It initializes the SMS sending process, prints the message for debugging purposes,
 * and then sends the message using the sms object.
 * The success of the send operation is determined based on the return values of the sms.beginSMS() and sms.endSMS() functions.
 * If the send operation is successful, the function returns true. Otherwise, it returns false.
 *
 * @note The function assumes that the necessary libraries and dependencies are included,
 * and the sms object for SMS communication is properly set up and initialized before calling this function.
 * The sendSMS flag from the configuration is not checked within this function, assuming it is already handled externally.
 */
bool SMS_sendMessage(char message[160], char remoteNumber[20]);



/**
 * @brief Upon UWQ or FWQ state, immediately sends an SMS to the operator.
 *
 * @param rmsClassArg The `rmsClass` object containing the water monitoring system information.
 * @param cfgStructArg The `ConfigurationStruct` object containing the configuration settings.
 * 
 * @details This function constructs and sends a SMS notification to the operator
 * It takes the `rmsClass` object and the `ConfigurationStruct` object as parameters.
 */
void SMS_wmImmediate(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Sends the SMS, if the RMS is within the HW
 *
 * @param rmsClassArg The `rmsClass` object containing the water monitoring system information.
 * @param cfgStructArg The `ConfigurationStruct` object containing the configuration settings.
 * @details This function constructs and sends an SMS to the operator. 
 * In case the state history percetange were unsuccessfully computed, the operator still receives an SMS, but is informed that the percentage failed
 * It takes the `rmsClass` object and the `ConfigurationStruct` object as parameters.
 *
 */
void SMS_wmHistoryWindow(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Sends an SMS notification for the URA event.
 *
 * @param rmsClassArg     An instance of the rmsClass representing the RMS state and data.
 * @param cfgStructArg    The configuration structure containing the SMS settings.
 *
 * @details This function constructs an SMS message containing the relevant information about the URA event,
 * including the RMS ID, date and time of the event, current state, and the last recorded ORP value.
 * The constructed message is then sent as an SMS notification if the 'ura' flag is set to true (for debugging purposes).
 *
 * @warning The function assumes that the necessary libraries and dependencies are included,
 * and the Tool_stringTime and SMS_sendSMS functions are defined separately.
 * @warning Please ensure that the SMS functionality is properly configured and initialized before calling this function.
 */
void SMS_uraSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Sends a Heartbeat (HB) SMS message.
 *
 * This function composes an HB SMS message using the current system information
 * and sends it via SMS if the HB feature is enabled.
 *
 * @param rmsClassArg The rmsClass object containing system information.
 * @param cfgStructArg The configuration settings for the system.
 * @return void
 * 
 * @details The function retrieves the current system time using the RTC and
 * formats it as a string. It then composes the HB SMS message using the system
 * information such as the RMS ID, state, and last ORP reading. If the HB feature
 * is enabled, the message is sent via SMS using the SMS_sendSMS() function.
 * 
 * @note This function assumes that the RTC object (rtc) is initialized and
 * configured correctly. It also relies on the Tool_stringTime() function to
 * format the system time as a string.
 * 
 * @warning The HB SMS message is sent only if the HB feature is enabled in the
 * configuration settings (cfgStructArg.hb).
 */
void SMS_hbSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);


/**
 * @brief Sends a Back-Up Power supply (BUP) SMS message indicating a change in power supply.
 *
 * @param rmsClassArg The rmsClass object containing system information.
 * @param cfgStructArg The configuration settings for the system.
 * @return void
 * 
 * @details  This function composes a BUP SMS message indicating whether the power supply has switched
 * from mains to external battery or from external battery to mains. The message is sent via SMS
 * if the BUP feature is enabled.
 * 
 * @note This function assumes that the SMS power structure (smsPowerStruct) in the rmsClass object
 * is correctly updated and reflects the current power supply status. It also relies on the
 * Tool_stringTime() function to format the timestamp as a string.
 * 
 * @warning The BUP SMS message is sent only if the BUP feature is enabled in the SMS.cpp
 */
void SMS_BUPSendIsStablePowerSupply(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

/**
 * @brief Sends a Back-Up Power supply (BUP) SMS message indicating the energy level of the battery.
 *
 * This function composes a BUP SMS message indicating the energy level of the battery, such as
 * when the critical energy level is reached or when the energy level is low or sufficient. The
 * message is sent via SMS if the BUP feature is enabled.
 *
 * @param rmsClassArg The rmsClass object containing system information.
 * @param cfgStructArg The configuration settings for the system.
 * @return void
 * 
 * @details The function retrieves the timestamp when the energy level SMS was sent and formats
 * it as a string using the Tool_stringTime() function. Depending on the energy level of the
 * battery stored in the power structure of the rmsClass object, the function composes the BUP
 * SMS message indicating the energy level and the battery voltage. The message is then sent via
 * SMS using the SMS_sendSMS() function if the BUP feature is enabled in the configuration settings.
 * 
 * @note This function assumes that the SMS power structure (smsPowerStruct) in the rmsClass object
 * is correctly updated and reflects the current energy level of the battery. It also relies on the
 * Tool_stringTime() function to format the timestamp as a string.
 * 
 * @warning The BUP SMS message is sent only if the BUP feature is enabled in the configuration
 * settings (cfgStructArg.bup).
 */
void SMS_BUPSendEnergyLevel(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg);

void SMS_deepSleepWakeUp (rmsClass& rmsClassArg, ConfigurationStruct& cfgStructArg);

#endif