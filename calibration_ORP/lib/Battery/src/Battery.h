
#ifndef BATTERY_H
#define BATTERY_H

#include "States.h"

/** 
 * @brief Initialisation sequence to ensure power supply (battery and mains) is suitable. 
 * 
 * @details If the battery is either
 * - not connected
 * - critical energy level (less then 5%) and the RMS is not connected to mains, 
 * stop the intialisation, and updates the rmsClass.
 * Also sets the powerStruct values (
 * - is stable power supply
 * - battery charge status
 * - battery voltage,
 * in the rms class
 * @param rms Class
 * @return bool of battery initialisation success
*/
bool Battery_runInitSequence(rmsClass& rmsClassArg);


/**
 * @brief Retrieves the voltage of the battery.
 *
 * This function reads the analog value from the ADC pin connected to the battery and converts it to the corresponding voltage.
 * It uses a voltage divider formula to calculate the battery voltage based on the ADC reading. The maximum source voltage
 * is determined using the resistor values R1 and R2. The calculated battery voltage is returned.
 *
 * @return The voltage of the battery.
 */
float Battery_getBatteryVoltage(void);


/**
 * @brief Calculates the battery percentage based on the battery voltage.
 *
 * This function takes the battery voltage as input and calculates the battery percentage using a linear mapping formula.
 * The battery voltage is mapped between the battery empty voltage and the battery full voltage to determine the percentage.
 * The calculated battery percentage is returned as a uint8_t value.
 *
 * @param batteryVoltage The voltage of the battery.
 * @return The battery percentage.
 */
uint8_t Battery_getBatteryPercentage(float batteryVoltage);


/**
 * @brief Checks if the power supply is stable.
 *
 * This function checks the stability of the power supply by querying the Power Management Integrated Circuit (PMIC).
 * It returns a boolean value indicating whether the power supply is stable or not.
 *
 * @return True if the power supply is stable, false otherwise.
 */
bool Battery_getIsStablePowerSupply();

/**
 * @brief Gets the current battery charge status.
 *
 * This function retrieves the current battery charge status by querying the Power Management Integrated Circuit (PMIC).
 * It returns a uint8_t value representing the charge status.
 *
 * @return The battery charge status.
 *         - 00 0000: Not charging
 *         - 01 0000: Pre-charge
 *         - 10 0000: Fast charging
 *         - 11 0000: Charge termination done
 */
uint8_t Battery_getChargeStatus();






#endif
