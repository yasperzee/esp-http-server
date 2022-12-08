/*************************build_json_docs.cpp***********************************

    Description:     Build responses

*******************************************************************************/
/* -----------------------------------------------------------------------------
    Version 0.4     Yasperzee   12'22   Add HC-SRO4 Ultrasonic Distance Sensor 
    Version 0.3     Yasperzee   12'22   Cleaning and refactoring
    Version 0.2     Yasperzee   11'22   IR Thermometer sensor support
    Version 0.1     Yasperzee   11'22   RPM measurement 

#TODO:
------------------------------------------------------------------------------*/
#include "ArduinoJson.h"
#include "build_json_docs.h"
#include "eeprom.h"
#include "read_sensors.h"
#include "ESP8266WiFi.h"
#include "setup.h"
//#include "read_sensors.h"


extern int reboots_eeprom_address; // EEPROM address to save reboots
extern uint8 wings_eeprom_address;

extern float rev;
extern float revTime;
//extern int wings;
extern int new_emissivity;
extern Values values;

extern localEeprom  eeprom_c;

ReadSensors read_sensors;

String buildJsonDocs::build_json_getdata_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
#ifdef SENSOR_TACOMETER
    values = read_sensors.get_rpm();
    root["RPM"] = values.rpm;
#elif defined SENSOR_IR_THERMOMETER
    read_sensors.get_ir_temperature();
    root["IR_TEMP_AMBIENT: "] = values.ir_ambient_temp;
    root["IR_TEMP_OBJECT: "] = values.ir_object_temp;
#elif defined SENSOR_ULTRASONIC_DISTANCE
    read_sensors.ReadUltrasonicSensor();
    root["DISTANCE: "] = values.distanceCm;
#endif
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String buildJsonDocs::build_json_getinfo_html(void) {
    String webpage;
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

String buildJsonDocs::build_json_getDebug_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
    //DynamicJsonDocument<500> root;
    //root["CORE_VERSION"] = ESP.getCoreVersion();
    //root["SDK_VERSION"] = ESP.getSdkVersion();
    root["APP_SW"] = APP_SW_VERSION;
    root["HardWare"] = HW_VERSION;
    root["NODE_FUNCTION"] = NODE_FUNCTION;
    root["SENSOR_MODEL_STR"] = SENSOR_MODEL_STR;
    int reboots = eeprom_c.read_eeprom(reboots_eeprom_address);
    //Serial.print("Reboots: ");
    //Serial.println(reboots);
#ifdef SENSOR_TACOMETER
    root["Revolutions: "] = rev;
    root["Rev.time: "] = revTime;
#elif defined SENSOR_IR_THERMOMETER
    // Add something if any...
#endif
    root["Reboots: "] = reboots;
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String buildJsonDocs::build_json_getSettings_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    //root["Settings"] = "GET n/a yet";
    root["Node function "] = NODE_FUNCTION;
    root["Sensor model "] = SENSOR_MODEL_STR;
#ifdef SENSOR_TACOMETER
   values.wings= eeprom_c.read_eeprom(wings_eeprom_address);
    root["RPM wings "] = values.wings;
#elif defined SENSOR_IR_THERMOMETER
    root["Emissivity "] = values.emissivity;
#endif
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String buildJsonDocs::build_json_putSettings_html(void) {
    String webpage;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Settings"] = " Update settins n/a yet";
    eeprom_c.write_eeprom(reboots_eeprom_address, 0);

    // things to updated wia PUT, saved to EEPROM, username & password required
        //  ssid, password  obsolete, handled by WiFiManager
        //  SENSOR_STR
        //  wings a.k.a ppr -> PulsesPerRevolution for Tachometer
        //  emissivity for IR Thermometer
        //  Sensor polarity NC, NO

        //  Hidden ones: reset Reboot counter
    
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

    void switch_light() {
    //Values ReadSensors::ReadUltrasonicSensor()
    read_sensors.ReadUltrasonicSensor();
    }

