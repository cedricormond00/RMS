#include <Arduino.h>

#include "SMS.h"

#include "Constant.h"
#include "Global.h"

#include "RTC.h"
#include "Tools.h"

bool wm = false;
bool ura = false;
bool hb = false;
bool bup = false;

void SMS_wmSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    if (rmsClassArg.get_wmAlarmSituation() == rmsClass::FIRSTANOMALY){
        SMS_wmImmediate(rmsClassArg, cfgStructArg);
        rmsClassArg.set_wmAlarmSituation(rmsClass::HWANOMALIES);
    }
    else if (rmsClassArg.get_wmAlarmSituation() == rmsClass::HWANOMALIES){
        SMS_wmHistoryWindow(rmsClassArg, cfgStructArg);
    }
    else if (rmsClassArg.get_wmAlarmSituation() == rmsClass::NORMALOCCURENCE){
        SMS_wmHistoryWindow(rmsClassArg, cfgStructArg);
        rmsClassArg.set_wmAlarmSituation(rmsClass::NOANOMALIES);
    }
    // else if (rmsClassArg.get_wmAlarmSituation() == 1){
    //     SMS_immediateWM(rmsClassArg);
    //     rmsClassArg.set_wmAlarmSituation(2);
    // }
}

void SMS_wmImmediate(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    Serial.println("");
    Serial.println("---");
    sprintf(message,"RMS %d\n"
                    // "%s\n"
                    "WQ Situation\n"
                    "Current State: %d\n"
                    "Last ORP reading: %.2f\n"
                    "Update will come in at regular interval until a SWQ is detected at the end of the History Window"
                    , RMS_ID, rmsClassArg.get_rmsState(), rmsClassArg.get_orpReading());
    Serial.println(message);
    Serial.println("---");
    if (wm){
        SMS_sendSMS(cfgStructArg, message);
    }    
    // Serial.println("");
    // Serial.println("---");
    // Serial.print("RMS ");
    // Serial.println(RMS_ID);
    // Serial.println("WQ situation");
    // Serial.print("Current State: ");
    // Serial.println(rmsClassArg.get_rmsState());
    // Serial.print("Last ORP reading: ");
    // Serial.println(rmsClassArg.get_orpReading());
    // Serial.println("Update will come in at regular interval until a SWQ is detected at the end of the History Window");
    // Serial.println("---");
}

void SMS_wmHistoryWindow(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    Serial.println("");
    Serial.println("---");
    sprintf(message,"RMS %d\n"
                    "WQ Situation\n"
                    "Current State: %d\n"
                    "Last ORP reading: %.2f\n"
                    "SWQ percentage: %.2f\n"
                    "UWQ percentage: %.2f\n"
                    "FWQ percentage: %.2f"
                    , RMS_ID, rmsClassArg.get_rmsState(), rmsClassArg.get_orpReading(),
                    rmsClassArg.get_stateHistoryPercentage(SWQ),
                    rmsClassArg.get_stateHistoryPercentage(UWQ),
                    rmsClassArg.get_stateHistoryPercentage(FWQ));
    Serial.println(message);
    Serial.println("---");
    if (wm){
        SMS_sendSMS(cfgStructArg, message);
    }    
    // Serial.println("");
    // Serial.println("---");
    // Serial.print("RMS ");
    // Serial.println(RMS_ID);
    // Serial.println("WQ situation");
    // Serial.print("Current State: ");
    // Serial.println(rmsClassArg.get_rmsState());
    // Serial.print("Last ORP reading: ");
    // Serial.println(rmsClassArg.get_orpReading());
    // Serial.print("SWQ percentage");
    // Serial.println(rmsClassArg.get_stateHistoryPercentage(SWQ),2);
    // Serial.print("UWQ percentage");
    // Serial.println(rmsClassArg.get_stateHistoryPercentage(UWQ),2);
    // Serial.print("FWQ percentage");
    // Serial.println(rmsClassArg.get_stateHistoryPercentage(FWQ),2);
    // Serial.println("---");
}

void SMS_uraSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    Serial.println("");
    Serial.println("---");
    sprintf(message,"RMS %d\n"
                    "URA Situation\n"
                    "Current State: %d\n"
                    "Last ORP reading: %.2f\n"
                    , RMS_ID,
                    rmsClassArg.get_rmsState(), 
                    rmsClassArg.get_orpReading());
    Serial.println(message);
    Serial.println("---");
    if (ura){
        SMS_sendSMS(cfgStructArg, message);
    }    
    // Serial.println("");
    // Serial.println("---");
    // Serial.print("RMS ");
    // Serial.println(RMS_ID);
    // Serial.println("URA situation");
    // Serial.print("Current State: ");
    // Serial.println(rmsClassArg.get_rmsState());
    // Serial.print("Last ORP reading: ");
    // Serial.println(rmsClassArg.get_orpReading());
    // Serial.println("---");
}


void SMS_hbSend(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char buf[40];
    RTC_getTimeInText(rtc.getEpoch(), buf);
    char message[160];
    Serial.println("");
    Serial.println("---");
    sprintf(message,"RMS %d\n"
                    "HB\n"
                    "Current State: %d\n"
                    "Last ORP reading: %.2f\n"
                    "Current time: %s"
                    , RMS_ID,
                    rmsClassArg.get_rmsState(), 
                    rmsClassArg.get_orpReading(),
                    buf);
    Serial.println(message);
    Serial.println("---");
    if (hb){
        SMS_sendSMS(cfgStructArg, message);
    }    
    // Serial.println("");
    // Serial.println("---");
    // Serial.print("RMS ");
    // Serial.println(RMS_ID);
    // Serial.println("HB");
    // Serial.print("Current State: ");
    // Serial.println(rmsClassArg.get_rmsState());
    // Serial.print("Last ORP reading: ");
    // Serial.println(rmsClassArg.get_orpReading());
    // Serial.print("Current Time: ");
    // RTC_getTimeInText(rtc.getEpoch(), buf);
    // Serial.println(buf);
    // Serial.println("---");
}





void SMS_BUPSendIsStablePowerSupply(rmsClass& rmsClassArg, ConfigurationStruct cfgStructArg){
    char message[160];
    if(rmsClassArg.get_smsPowerStructIsStablePowerSupply()){
        /*TODO: could add a further test to ensure that the energy level of the battery is sufficiently high.
        if it is, then send this sms. Ohterwise, dont send the sms, and wait for the next test to send both info at the same time
        */
        Serial.println("");
        Serial.println("---");
        sprintf(message,"RMS %d\n"
                        "BUP situation\n"
                        "power source switched from mains to external battery"
                        , RMS_ID);
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
        sprintf(message,"RMS %d\n"
                        "BUP situation back to normal\n"
                        "power source switched from external battery to mains"
                        , RMS_ID);
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

    switch(rmsClassArg.get_powerStructBatteryELState())
    {
    case rmsClass::criticalEL :
        Serial.println("");
        Serial.println("---");
        sprintf(message,"RMS %d\n"
                        "Critical Energy Level reached\n"
                        "Device will go to deepsleep"
                        , RMS_ID);
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
        sprintf(message,"RMS %d\n"
                        "Low Energy Level reached"
                        , RMS_ID);
        Serial.println(message);
        Serial.println("---");

        // Serial.println("---");
        // Serial.print("RMS ");
        // Serial.println(RMS_ID);
        // Serial.println("Low Energy Level reached");
        // // Serial.println("Device will soon go to deepsleep");
        // Serial.println("---");
        break;
    case rmsClass::sufficientEL :
        Serial.println("");
        Serial.println("---");
        sprintf(message,"RMS %d\n"
                        "Sufficient Energy Level reached"
                        , RMS_ID);
        Serial.println(message);
        Serial.println("---");

        // Serial.println("---");
        // Serial.print("RMS ");
        // Serial.println(RMS_ID);
        // Serial.println("Sufficient Energy Level reached");
        // Serial.println("---");
        break;
    default:
        break;
    }
    Serial.println();
    Serial.println("test break");
    Serial.println();

    if (bup){
        SMS_sendSMS(cfgStructArg, message);
    }
}

bool SMS_initConnection(){
    // connection state
    bool connected = false;

    // If your SIM has PIN, pass it as a parameter of begin() in quotes
    //TODO: change the while to a count up to 5 tries
    while (!connected) {
        // if (nbAccess.begin(PINNUMBER) == NB_READY) {
            //here is the problem
        NB_NetworkStatus_t status = nbAccess.status();
        Serial.print("NB status: ");
        Serial.println(status);
        if (status == NB_READY) {
            connected = true;
        }
        else {
            Serial.println("Not connected");
            delay(1000);
            Serial.println("NB not NB_ready. SHutting down and rebegining.");
            nbAccess.shutdown();
            nbAccess.begin(PINNUMBER);
            status = nbAccess.status();
            Serial.println("new current status: ");
            Serial.print(status);
        }    
    } 



    // while (!connected) {
    //     if (nbAccess.begin(PINNUMBER) == NB_READY) {
    //         connected = true;
    //     } 
    //     else {
    //         Serial.println("Not connected");
    //         delay(1000);
    //     }
    // }
    Serial.println("NB initialized");
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

void SMS_sendSMS(ConfigurationStruct cfgStructArg, char message[160]){
    if (cfgStructArg.sendSMS){
        if (SMS_initConnection()){
            SMS_sendMessage(message,cfgStructArg.remoteNumber);
        }
    }
}