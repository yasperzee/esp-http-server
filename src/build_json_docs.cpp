/*************************build_json_docs.cpp**********************************

    Description:    
*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.1     Yasperzee   11'22 

#TODO:
------------------------------------------------------------------------------*/

#include "build_json_docs.h"

String build_json_getdata_html(void)
    {
    Values values;
    String webpage;

    values = read_dht_sensor();

    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Temp"] = values.temperature;
    root["Humid"] = values.humidity;
    root["RPM"] = "n/a";
    //root["RSSI"] = WiFi.RSSI();
    //root["VCC"] = ESP.getVcc();
    //root["INFO"] = "ESP-01s";

    //Store JSON in String variable  
    serializeJson(root, webpage);

    //Serial.println("webpage: ");
    //Serial.println(webpage);

    return webpage;
    }

String build_json_getinfo_html(void)
    {

    String webpage;
    Values values;

    values = read_dht_sensor();

    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    //root["Temp"] = values.temperature;
    //root["Humid"] = values.humidity;
    root["RSSI"] = WiFi.RSSI();
    root["MAC"] = WiFi.macAddress();
    root["VCC"] = ESP.getVcc();
    root["CHIPID"] = ESP.getChipId();
    root["NODETYPE"] = "ESP-01s";

    //Store JSON in String variable  
    serializeJson(root, webpage);

    //Serial.println("webpage: ");
    //Serial.println(webpage);

    return webpage;
    }
