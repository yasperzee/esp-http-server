
/*************************debug.cpp*********************************************

    Description:

*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.1     Yasperzee   12'22   

#TODO:
------------------------------------------------------------------------------*/

#include "debug.h"
#include "setup.h"

extern int wings;

void printInfo() {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: "), WiFi.RSSI();
    Serial.println(WiFi.RSSI());
    Serial.print("MAC: ");  
    Serial.println(WiFi.macAddress()); 
    Serial.print("Vcc: ");  
    Serial.println(ESP.getVcc());
    Serial.print("Core version: "); 
    Serial.println(ESP.getCoreVersion()); //returns a String containing the core version.
    Serial.print("Chip ID: "); 
    Serial.println(ESP.getChipId()); //returns the ESP8266 chip ID as a 32-bit intege
    Serial.print("SDK version: ");
    Serial.println(ESP.getSdkVersion());ESP.getSdkVersion(); //returns the SDK version as a char.
    Serial.print("Node is ");
    Serial.println(NODEMCU_STR);
    Serial.print("APP_SW_VERSION is ");
    Serial.println(APP_SW_VERSION);
    Serial.print("HW_VERSION is ");
    Serial.println(HW_VERSION);
    Serial.print("Node_function");
    Serial.println(NODE_FUNCTION);
    Serial.print("Wings is ");
    Serial.println(wings);
    }