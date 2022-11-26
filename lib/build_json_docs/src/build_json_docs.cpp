/*************************build_json_docs.cpp**********************************

    Description:     Build responses

*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.1     Yasperzee   11'22   RPM measurement 

#TODO:
------------------------------------------------------------------------------*/

#include "build_json_docs.h"

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

    return webpage;
    }

String build_json_getDebug_html(void) {
    String webpage;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Reboots"] = 5; // should be saved to EEPROM
    
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String build_json_getSettings_html(void) {
    String webpage;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Settings"] = "GET n/a yet";

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
    root["Settings"] = " PUT n/a yet";

    // things to updated wia PUT, saved to EEPROM
    // ssid, password
    // SENSOR_STR
    
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }
