// I2c.h
#ifndef I2C_H // include guard
#define I2C_H


/**  \brief initializes the Wire library and join the I2C bus as a controller or a peripheral. 
 * \remark This function should normally be called only once.
 * \remark PLace it in the setup function
*/
void I2c_init (void);

/**  \brief initializes the Wire library and join the I2C bus as a controller or a peripheral. 
 * \param i2cAddress: address we want to communicate with 
 * \param command = command to be sent (String) 
*/
void I2c_sendCommand(uint8_t i2cAddress, const char *command_);

/**  \brief sends a command to the peripheral ORP senosor
 * \param command_ = command code (String) we want to send to the ORP EZO circuit
*/
void I2c_sendCommandORP(char command_[]);

/**  \brief REceives the reply after a command sent to the EZO ORP Circuit
 * \param ORPData_ = holds the returned value from the EZO ORP CIrcuit
 * \param command_ = command sent to the EZO ORPO Cicruit (just to check it is not sleep)
 * \param responseDelay = item to hold the required delay to wait for the EZO ORP circuit reply
*/
void I2c_receiveORPData(char ORPData_[], char command_[], int responseDelay);

/**  \brief checks the response delay required for proper functionning of i2c communication with the EZO ORP circuit
 * \param ORPData_ = holds the returned value from the EZO ORP CIrcuit
 * \param command_ = command sent to the EZO ORPO Cicruit (just to check it is not sleep)
 * \param commandLength_ = constant value which holds the maximum length of the command sent to the sensor
*/
int I2c_checkResponseDelay(char command_[], uint8_t commandLength_);


/**  \brief sends a command and receives the response to the ORP sensor 
 * \remark more granular than I2c_sendCommandToSensor
 * \param ORP_data = char array, to hold the response from the sensor
 * \param command = char array, to hold the command sent
 * \param commandLength = constant value which holds the maximum length of the command sent to the sensor
*/
void I2c_sendReceiveORP(char ORPData_[], char command_[], uint8_t commandLength_);


/**  \brief sends a command to the sensor, and waits for its reply
 * \param ORP_data = char array, to hold the response from the sensor
 * \param command = char array, to hold the command sent
 * \param commandLength = constant value which holds the maximum length of the command sent to the sensor
*/
void I2c_sendCommandToSensor(char ORPData_[], char command_[], uint8_t commandLength_ );                                                 //if a command was sent to the EZO device.



#endif