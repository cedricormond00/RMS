#include <Arduino.h>

#include "SMS.h"

#include "Constant.h"
#include "Global.h"

#include "RTC.h" //TODO: comment out
#include "Tool.h"

bool wm = false;
bool ura = false;
bool hb = false;
bool bup = true;

void SMS_init(uint32_t timeout){
    nbAccess.setTimeout(timeout); 
}

void SMS_sendSMS(ConfigurationStruct cfgStructArg, char message[160]){
    if (cfgStructArg.sendSMS){
        if (SMS_initConnection()){
            SMS_sendMessage(message,cfgStructArg.remoteNumber);
        }
    }
}

bool SMS_initConnection(){
    // connection state
    bool connected = false;
    // number of conenction to network attempts
    uint8_t count = 0;

    while (!connected && count <5) {
        NB_NetworkStatus_t status = nbAccess.status();
        Serial.print("NB status: ");
        Serial.println(status);
        if (status == NB_READY) {
            connected = true;
            Serial.println("NB initialized");
        }
        else {
            Serial.println("Not connected");
            count ++;
            delay(1000);
            if (count!=0){
                Serial.println("NB not NB_ready. Shutting down and rebegining.");
                Serial.print("Connection attempt: ");
                Serial.println(count);
            }
            nbAccess.shutdown();
            nbAccess.begin(PINNUMBER);
            status = nbAccess.status();
        }    
    } 
    return connected;
}

bool SMS_sendMessage(char message[160], char remoteNumber[20]){
    bool success = true;
    // if (sendSMS){
    Serial.println("SENDING");
    Serial.println();
    Serial.println("Message:");
    Serial.println(message);

    // send the message
    success = sms.beginSMS(remoteNumber);
    if (success == 1){
        success = sms.print(message);
    } else{
        //TODO: could return an error code to know where it failed
        return success;
    }

    if (success == 1){
        success = sms.endSMS();
    } else{
        //TODO: could return an error code to know where it failed
        return success;
    }
    
    Serial.println("\nCOMPLETE!\n");
    return success;
    // }
}


void SMS_wmSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    // first detection of an anomaly
    if (rmsClassArg.get_wmAlarmSituation() == rmsClass::FIRSTANOMALY){
        SMS_wmImmediate(rmsClassArg, cfgStructArg);
        // now set the alarm situation as being inside the HistoryWindow of anomalies
        rmsClassArg.set_wmAlarmSituation(rmsClass::HWANOMALIES);
    }
    // we shall remain in the histoy window
    else if (rmsClassArg.get_wmAlarmSituation() == rmsClass::HWANOMALIES){
        SMS_wmHistoryWindow(rmsClassArg, cfgStructArg);
    }
    // We may now exit the history window, because the criterias to qualify the RMS as back to safe mode were met
    else if (rmsClassArg.get_wmAlarmSituation() == rmsClass::NORMALOCCURENCE){
        SMS_wmHistoryWindow(rmsClassArg, cfgStructArg);
        //exit the HW SMS sending
        rmsClassArg.set_wmAlarmSituation(rmsClass::NOANOMALIES);
    }
}


void SMS_wmImmediate(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    char dateTime[21];
    Tool_stringTime(rmsClassArg.get_wmReadEPochTime(), dateTime);
    Serial.println("");
    Serial.println("---");
    sprintf(message,"%d\n" //+ 1
                    "%s\n" //+ 1
                    "WQ\n" //2 + 1
                    "State %d\n" //6 + 1
                    "Last ORP %.2f\n" //9 + 1
                    "Regular updates until SWQ at end of HW" //38
                    // , RMS_ID, dateTime, rmsClassArg.get_rmsState(), rmsClassArg.get_orpReading());
                    , RMS_ID, dateTime, rmsClassArg.get_rmsState(), rmsClassArg.get_orpReading());
                    // 2 + 21 + 1 + 7
                    // = 90
    Serial.println(message);
    Serial.println("---");
    if (wm){
        SMS_sendSMS(cfgStructArg, message);
    }    
}


void SMS_wmHistoryWindow(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    char dateTime[21];
    Tool_stringTime(rmsClassArg.get_wmReadEPochTime(), dateTime);

    Serial.println("");
    Serial.println("---");
    // if we were successful in updating the state History
    if (rmsClassArg.get_stateHistorySuccess()){
        sprintf(message,"%d\n"//+ 1
                        "%s\n"//+ 1
                        "WQ\n"//2 + 1
                        "State %d\n"//6 + 1
                        "Last ORP %.2f\n"//9 + 1
                        "SWQ %.2f\n"//4 + 1
                        "UWQ %.2f\n"//4 + 1
                        "FWQ %.2f"//4 + 1
                        , RMS_ID, // 2
                        dateTime,//21
                        rmsClassArg.get_rmsState(), //1
                        rmsClassArg.get_orpReading(), //7
                        rmsClassArg.get_stateHistoryPercentage(SWQ), //4
                        rmsClassArg.get_stateHistoryPercentage(UWQ), //4
                        rmsClassArg.get_stateHistoryPercentage(FWQ)); //4
                        // 80
        Serial.println(message);
    }
    // if we were unsucceessful in updating the state History
    else{
        sprintf(message,"%d\n"//+ 1
                "%s\n"//+ 1
                "WQ\n"//2 + 1
                "State %d\n"//6 + 1
                "Last ORP %.2f\n"//9 + 1
                "Failed to compute states' percentage over the HW"//46 + 1
                , RMS_ID, // 2
                dateTime,//21
                rmsClassArg.get_rmsState(), //1
                rmsClassArg.get_orpReading(), //7
                rmsClassArg.get_stateHistoryPercentage(SWQ), //4
                rmsClassArg.get_stateHistoryPercentage(UWQ), //4
                rmsClassArg.get_stateHistoryPercentage(FWQ)); //4
                // 112
        Serial.println(message);  
    } 
    Serial.println("---");
    if (wm){
        SMS_sendSMS(cfgStructArg, message);
    }    
}


void SMS_uraSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    char dateTime[21];
    Tool_stringTime(rmsClassArg.get_URAcurrentAlarmEPochTime(), dateTime);
    Serial.println("");
    Serial.println("---");
    sprintf(message,"%d\n"//+ 1
                    "%sd\n"
                    "URA\n"//3 + 1
                    "State %d\n"//6 + 1
                    "Last ORP %.2f\n"//9 + 1
                    , RMS_ID, //2
                    dateTime, //21
                    rmsClassArg.get_rmsState(), //1
                    rmsClassArg.get_orpReading()); //7
                    //53
    Serial.println(message);
    Serial.println("---");
    if (ura){
        SMS_sendSMS(cfgStructArg, message);
    }    
}


void SMS_hbSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char buf[40];
    Tool_stringTime(rtc.getEpoch(), buf);
    char message[160];
    Serial.println("");
    Serial.println("---");
    sprintf(message,"%d\n"//+ 1
                    "%s\n"//+1
                    "HB\n"//2 + 1
                    "State %d\n"//6 + 1
                    "Last ORP %.2f\n"//9 + 1
                    , RMS_ID,//2
                    buf,//40
                    rmsClassArg.get_rmsState(),//1
                    rmsClassArg.get_orpReading());//7
                    //72
    Serial.println(message);
    Serial.println("---");
    if (hb){
        SMS_sendSMS(cfgStructArg, message);
    }    
}


void SMS_BUPSendIsStablePowerSupply(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    char dateTime[21];
    // use wmreadEPochTime becuase we update just after both the stablePowerSUpplyStauts and time after the emssage was sent, at once
    
    // put in human readable format the epoch time at which the stable s^power supply was performed
    Tool_stringTime(rmsClassArg.get_smsPowerStructIsStablePowerSupplySMSSentEPochTime(), dateTime);
    /*
    Check whether we have sent an SMS informing the power is unstable. (battery is now the power supply)
    This implies that the smsPowerStructIsStablePowerSupply is true: 
    the RMS has not yet sent an SMS to inform that there is unstable power supply: 
    it still thinks the power source is stable, hence why true
    */
    if(rmsClassArg.get_smsPowerStructIsStablePowerSupply()){
        /*TODO: could add a further test to ensure that the energy level of the battery is sufficiently high.
        if it is, then send this sms. Ohterwise, dont send the sms, and wait for the next test to send both info at the same time
        REPLY: For the operator, it might be easier if they get separate SMS, one per situation, to differentiate more easily
        */
        Serial.println("");
        Serial.println("---");
        sprintf(message,"%d\n"//+ 1
                        "%s\n"//+ 1
                        "BUP\n"//3 + 1
                        "Power supply switched from mains to external battery\n"//51
                        "Battery voltage %.2f"//16
                        , RMS_ID, //2
                        dateTime,//21
                        rmsClassArg.get_powerStructBatteryVoltage());//4
                        //100
        Serial.println(message);
        Serial.println("---");
        
        // Serial.println("---");
        // Serial.print("RMS ");
        // Serial.println(RMS_ID);
        // Serial.println("BUP situation");
        // Serial.println("power source switched from mains to external battery");
        // Serial.println("---");
    }
    else{
        
        Serial.println("");
        Serial.println("---");
        sprintf(message,"%d\n"//+ 1
                        "%s\n"//+ 1
                        "BUP\n"//3 + 1
                        "Power supply switched from external battery to mains"//51
                        , RMS_ID, //2
                        dateTime);//21
                        //80
        Serial.println(message);
        Serial.println("---");
       

        // Serial.println("---");
        // Serial.print("RMS ");
        // Serial.println(RMS_ID);
        // Serial.println("BUP situation back to normal");
        // Serial.println("power source switched from external battery to mains");
        // Serial.println("---");
    }
    if (bup){
        SMS_sendSMS(cfgStructArg, message);
    }
}


void SMS_BUPSendEnergyLevel(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    char dateTime[21];

    Tool_stringTime(rmsClassArg.get_smsPowerStructEnergyLevelSMSSentEPochTime(), dateTime);
    
    switch(rmsClassArg.get_powerStructBatteryELState())
    {
    case rmsClass::criticalEL :
        Serial.println("");
        Serial.println("---");
        sprintf(message,"%d\n"//+ 1
                        "%s\n"//+ 1
                        "Critical Energy Level reached\n"//29 + 1
                        "Device will go to deepsleep"//27
                        "Battery voltage %.2f"//16
                        , RMS_ID,//2
                        dateTime,//21
                        rmsClassArg.get_powerStructBatteryVoltage());//4
                        //106
        Serial.println(message);
        Serial.println("---");
        
        // Serial.println("---");
        // Serial.print("RMS ");
        // Serial.println(RMS_ID);
        // Serial.println("Critical Energy Level reached");
        // Serial.println("Device will go to deepsleep");
        // Serial.println("---");
        break;

    case rmsClass::lowEL :
        Serial.println("");
        Serial.println("---");
        sprintf(message,"%d\n"//+ 1
                        "%s\n"//+ 1
                        "Low Energy Level reached"//25
                        "Battery voltage %.2f"//16
                        , RMS_ID,//2
                        dateTime,//21
                        rmsClassArg.get_powerStructBatteryVoltage());//4
                        //70
        Serial.println(message);
        Serial.println("---");
        break;

    case rmsClass::sufficientEL :
        Serial.println("");
        Serial.println("---");
        sprintf(message,"%d\n"//+ 1
                        "%s\n"//+ 1
                        "Sufficient Energy Level reached"//31
                        "Battery voltage %.2f"//16
                        , RMS_ID,//2
                        dateTime,//21
                        rmsClassArg.get_powerStructBatteryVoltage());//4
                        //77
        Serial.println(message);
        Serial.println("---");
        break;

    default:
        break;

    }
    if (bup){
        SMS_sendSMS(cfgStructArg, message);
    }
}



