// Tool.h
#ifndef TOOL_H // include guard
#define TOOL_H



/**
 * @brief Converts the given epoch time to a formatted string representation (yyyy/mm/dd-hh:mm:ss).
 *
 * This function converts the provided epoch time to a human-readable string format
 * and stores it in the given buffer.
 *
 * @param ePochTime The epoch time value to convert.
 * @param buf The buffer to store the formatted string representation.
 */
void Tool_stringTime(uint32_t ePochTime, char* buf);


void Tools_print2digits(int number);




/** \brief obtains the next power of two, for a 16 bit integer
 * \param v_ = input 16 bit integer
*/
uint16_t Tool_16bitNextPow2(uint16_t v_);

/** \brief converts a period to a frequency
 * \param t_ = input 32 bit integer
*/
float Tool_periodToFrequency(uint32_t t_);

/** \brief Sets the bits off where 1 are in the mask
 * \param byte = input 8 bit word. To be had bits turned off
 * \param mask = mask which contains the bits to turn off
*/
void Tool_setBitOff(uint8_t* byte, uint8_t mask);
void Tool_setBitOff(volatile uint8_t* byte, uint8_t mask);

/** \brief Sets the bits on where 1 are in the mask
 * \param byte = input 8 bit word. To be had bits turned on
 * \param mask = mask which contains the bits to turn on
*/
void Tool_setBitOn(uint8_t* byte, uint8_t mask);
void Tool_setBitOn(volatile uint8_t* byte, uint8_t mask);


/** \brief Checks if specific bit sin mask are on or not. 
 * \param byte = input 8 bit word. Contains bits to be compared
 * \param mask = mask which contains the bits to compare with
*/
bool Tool_isBitOn(uint8_t byte, uint8_t mask);
// void Tool_isBitOn(volatile uint8_t* byte, uint8_t mask);






// class RingBuffer {
// private:
//     float* buffer;
//     int bufferSize;
//     int currentIndex;

// public:
//     RingBuffer(int size);
//     ~RingBuffer();
//     void addToBuffer(float value);
//     float getOldestValue();
//     float getNewestValue();
// };

#endif