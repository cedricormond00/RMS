#include <Arduino.h>

#include <iot_cmd.h>
#include <Ezo_i2c_util.h>  

#include "EZO.h"

void EZO_getEzoORPReading(Ezo_board& ezoClassArg){
    ezoClassArg.send_read_cmd();
    delay(815); // delay required for reading command

    // TODO: eventually, will need to create my own fuction that only reads the values and that can then be used to store
    receive_and_print_reading(ezoClassArg);

    // put device to sleep
    ezoClassArg.send_cmd("Sleep");

}