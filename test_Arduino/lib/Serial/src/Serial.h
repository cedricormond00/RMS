

/** \brief checks if there has been a command sent through the terminal
 * \param computerData_ = data command sent from computer
 * \param computerDataLength_ = length of data command sent from computer 
 * \remark isntead of placing this in a loop, I would rather having the terminal trigger an interrupt
*/
void serialEvent(char computerData_[], uint8_t computerDataLength_);
