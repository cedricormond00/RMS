// I2c.h
#ifndef I2C_H // include guard
#define I2C_H


/**  \brief initializes the Wire library and join the I2C bus as a controller or a peripheral. 
 * \remark This function should normally be called only once.
 * \remark PLace it in the setup function
*/
void I2c_init (void);

/**  \brief sends a command to the sensor, and waits for its reply
 * \param ORP_data = char array, to hold the response from the sensor
 * \param command = char array, to hold the command sent
 * \param commandLength = constant value which holds the maximum length of the command sent to the sensor
*/
void I2c_sendCommandToSensor(char ORPData_[], char command_[], uint8_t commandLength_ );                                                 //if a command was sent to the EZO device.

/**  \brief sends a command and receives the response to the ORP sensor 
 * \remark more granular than I2c_sendCommandToSensor
 * \param ORP_data = char array, to hold the response from the sensor
 * \param command = char array, to hold the command sent
 * \param commandLength = constant value which holds the maximum length of the command sent to the sensor
*/
void I2c_sendReceiveORP(char ORPData_[], char command_[], uint8_t commandLength_);



#endif