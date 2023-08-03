// EZO.h
#ifndef EZO_H // include guard
#define EZO_H


#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib

/**
 * @brief Retrieves the ORP reading from the EZO ORP sensor.
 *
 * 
 * This function sends a read command to the EZO ORP sensor, retrieves the ORP reading,
 * and puts the device to sleep afterwards. The function assumes that the necessary libraries
 * and dependencies are included, and the EZO.h library and Ezo_board class are correctly defined.
 * 
 * Note: The delay of 815 milliseconds after sending the read command is required to allow sufficient time for the
 * sensor to respond with the ORP reading.
 * @param ezoClassArg An instance of the Ezo_board class representing the EZO ORP sensor.
 * 
 * @warning Please ensure that the I2C communication with the sensor has been properly initialized before calling this function.
 * @warning Additionally, the function assumes that the `receive_and_print_reading` function, which is used to receive and
 * store the ORP values, has been defined separately.

 */
void EZO_getEzoORPReading(Ezo_board& ezoClassArg);

#endif