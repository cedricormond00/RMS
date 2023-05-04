// I2c.h
#ifndef I2C_H // include guard
#define I2C_H

/**  \brief sends a command to the sensor, and waits for its reply
 * \param ORP_data = char array, to hold the response from the sensor
 * \param command = char array, to hold the command sent
 * \param commandLength = constant value which holds the maximum length of the command sent to the sensor
*/
void I2c_sendCommandToSensor(char ORPData_[], char command_[], uint8_t commandLength_ );                                                 //if a command was sent to the EZO device.

#endif