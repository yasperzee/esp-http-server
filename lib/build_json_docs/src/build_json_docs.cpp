/*************************build_json_docs.cpp**********************************

    Description:     Build responses

*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.1     Yasperzee   11'22   RPM measurement 

#TODO:
------------------------------------------------------------------------------*/

#include "build_json_docs.h"
//#include "ssid.h"  // SSID and PASS strings for local network

extern int reboots_eeprom_address; // address to save reboots
extern void write_eeprom( int addr, int value);
extern int read_eeprom(int address);

extern float rev;
extern float revTime;
extern float wings;

String build_json_getdata_html(void) {
    //Values values;
    String webpage;

    //values= read_dht_sensor();
    float r= get_rpm();
    float rpm = roundf(r * 100) / 100; // 2 decimals 

    StaticJsonDocument<500> root;
    // DynamicJsonDocument<500> root;
    //root["RPM"] = values.rpm;
    root["RPM"] = rpm;
   
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String build_json_getinfo_html(void) {
    String webpage;
    // Values values;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["RSSI"] = WiFi.RSSI();
    root["NODE_IP"] = WiFi.localIP();
    //root["MAC"] = WiFi.macAddress();
    //root["VCC"] = ESP.getVcc();
    root["CHIPID"] = ESP.getChipId();
    //root["CORE_VERSION"] = ESP.getCoreVersion();
    //root["SDK_VERSION"] = ESP.getSdkVersion();
    root["APP_SW"] = APP_SW_VERSION;
    //root["NODETYPE"] = NODEMCU_STR;
    
    //Store JSON in String variable  
    serializeJson(root, webpage);
    Serial.print("Info  : ");
    Serial.println(webpage);

    return webpage;
    }

String build_json_getDebug_html(void) {
    String webpage;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    // read reboots count from EEPROM, increment and write back
    int reboots = read_eeprom(reboots_eeprom_address);
    Serial.print("Reboots: ");
    Serial.println(reboots);
    root["Reboots: "] = reboots;
    root["Revolutions: "] = rev;
    root["Rev.time: "] = revTime;

    //Store JSON in String variable  
    serializeJson(root, webpage);
    return webpage;
    }

String build_json_getSettings_html(void) {
    String webpage;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    //root["Settings"] = "GET n/a yet";
    root["SSID"] = "ssid";
    root["Router pwd"] = "********";
    root["Sensor "] = SENSOR_STR;
    root["RPM wings "] = wings;
 
    // things to updated wia PUT, saved to EEPROM
    // ssid, password
    // SENSOR_STR
    
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String build_json_putSettings_html(void) {
    String webpage;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Settings"] = " Update settins n/a yet";

    // things to updated wia PUT, saved to EEPROM, username & password required
        //  ssid, password
        //  SENSOR_STR
        //  wings a.k.a ppr PulsesPerRevolution
        //  Sensor polarity NC, NO
        //  Hidden ones: reset Reboot counter
        //              reset  ssid & password
    
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }
