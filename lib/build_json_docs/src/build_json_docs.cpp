/*************************build_json_docs.cpp**********************************

    Description:    
*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.1     Yasperzee   11'22   RPM measurement 

#TODO:
------------------------------------------------------------------------------*/

#include "build_json_docs.h"

String build_json_getdata_html(void)
    {
    Values values;
    String webpage;

    //values= read_dht_sensor();
    float rpm= get_rpm();

    StaticJsonDocument<500> root;
    // DynamicJsonDocument<500> root;

    //root["Temp"] = values.temperature;
    //root["Humid"] = values.humidity;
    //root["PIN_STATE"] = state;
    //root["RPM"] = values.rpm;
    root["RPM"] = rpm;
    //root["DebugREV"] = rpm;
    
    //Store JSON in String variable  
    serializeJson(root, webpage);

    //Serial.println("webpage: ");
    //Serial.println(webpage);

    return webpage;
    }

String build_json_getinfo_html(void)
    {

    String webpage;
   // Values values;

//    values = read_dht_sensor();
    //Values = read_rpm();
    
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

    //Serial.println("webpage: ");
    //Serial.println(webpage);

    return webpage;
    }
