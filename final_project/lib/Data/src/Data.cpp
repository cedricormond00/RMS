#include <Arduino.h>
#include <SD.h>

#include "Data.h"

#include "Constant.h"
#include "States.h"


/*TODO:
- datafilename
- */
bool Data_saveDataPointToFile(RMSClass& rmsClassArg, uint8_t inputEventCode){
   bool success = true;
   char buf[256];
   File dataFile = SD.open(datafilename, FILE_WRITE);
   
   if (dataFile) {
      t=now();
      getCurrentTimeInText(ePochTime, buf);
      dataFile.print(buf);
      dataFile.print(",");

      dataFile.print(data.tempRaw);
      dataFile.print(",");
      dataFile.print(data.t,4);
      dataFile.print(",");

      dataFile.print(data.phRaw);
      dataFile.print(",");
      dataFile.print(data.phmV,4);
      dataFile.print(",");    
      dataFile.print(data.ph,4);
      dataFile.print(",");

      dataFile.print(data.orpRaw);
      dataFile.print(",");
      dataFile.print(data.orpmV,4);
      dataFile.print(",");    
      dataFile.print(data.orp,4);
      dataFile.print(",");

      dataFile.print(data.ecRealRaw);
      dataFile.print(",");
      dataFile.print(data.ecImagRaw);
      dataFile.print(",");
      dataFile.print(data.ec,7);
      dataFile.print(",");
      dataFile.print(fBatteryVoltage,2);      
      dataFile.println();
      dataFile.close();
   } 
   else {
      success = false;
   }
   dataFile.close();  
   return success;
}
