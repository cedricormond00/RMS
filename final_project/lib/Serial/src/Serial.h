

/** \brief checks if there has been a command sent through the terminal
 * \param computerData_ = data command sent from computer
 * \param computerDataLength_ = length of data command sent from computer 
 * \remark isntead of placing this in a loop, I would rather having the terminal trigger an interrupt
*/
void serialEvent(char computerData_[], uint8_t computerDataLength_);

/** \brief debug function to help identify the content of the data sent over from the terminal
 * \param computerData_ = data command sent from computer
 * \param nCharReceivedFromComputer_ = number of characters sent from the computer 
 * \remark utility functiont to debug Serial.cpp
*/
void TextDescription(char computerData_[], uint8_t nCharReceivedFromComputer_);
