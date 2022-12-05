/*************************build_json_docs.cpp***********************************

    Description:     Builds responses

*******************************************************************************/

/* -----------------------------------------------------------------------------

    Version 0.3     Yasperzee   12'22   Cleaning and refactoring
    Version 0.2     Yasperzee   11'22   IR TEMPERATURE sensor support
    Version 0.1     Yasperzee   11'22   RPM measurement 

#TODO:
------------------------------------------------------------------------------*/
#include "build_json_docs.h"

extern int reboots_eeprom_address; // address to save reboots
extern void write_eeprom( int addr, int value);
extern int read_eeprom(int address);

extern float rev;
extern float revTime;
extern int wings;
extern int new_emissivity;
extern Values values;

ReadSensors read_sensors;

String build_json_getdata_html(void) {
    //Values values;
    String webpage;
    StaticJsonDocument<500> root;
#ifdef SENSOR_RPM
    float r=  read_sensors.get_rpm();
    float rpm = roundf(r * 100) / 100; // 2 decimals 
    StaticJsonDocument<500> root;
    // DynamicJsonDocument<500> root;
    root["RPM"] = values.rpm;
    //root["RPM"] = rpm;
#elif defined SENSOR_IR_TEMPERATURE
    read_sensors.get_ir_temperature();
    root["IR_TEMP_AMBIENT: "] = values.ir_ambient_temp;
    root["IR_TEMP_OBJECT: "] = values.ir_object_temp;
#endif
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
    //DynamicJsonDocument<500> root;
    //root["CORE_VERSION"] = ESP.getCoreVersion();
    //root["SDK_VERSION"] = ESP.getSdkVersion();
    root["APP_SW"] = APP_SW_VERSION;
    root["HardWare"] = HW_VERSION;
    root["NODE_FUNCTION"] = NODE_FUNCTION;
    root["SENSOR_MODEL_STR"] = SENSOR_MODEL_STR;
    
    int reboots = read_eeprom(reboots_eeprom_address);
    Serial.print("Reboots: ");
    Serial.println(reboots);
    root["Reboots: "] = reboots;
#ifdef SENSOR_RPM
    root["Revolutions: "] = rev;
    root["Rev.time: "] = revTime;
#elif defined SENSOR_IR_TEMPERATURE
    // Add something if any...
#endif
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String build_json_getSettings_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    //root["Settings"] = "GET n/a yet";
    root["Node function "] = NODE_FUNCTION;
    root["Sensor model "] = SENSOR_MODEL_STR;
#ifdef SENSOR_RPM
   root["RPM wings "] = wings;
#elif defined SENSOR_IR_TEMPERATURE
    root["Emissivity "] = new_emissivity;
#endif
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
