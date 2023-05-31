// Data.h
#ifndef DATA_H // include guard
#define DATA_H

#include "States.h"

bool Data_saveDataPointToFile(uint32_t ePochTime,
                                float orpValue,
                                RMSState evaluatedState,
                                uint8_t inputEventCodeBit, 
                                char datafilename[]);
                                

bool Data_SDCard_init( void );



#endif