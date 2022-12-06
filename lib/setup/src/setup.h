#ifndef SETUP_H
#define SETUP_H

/*************************** setup.h *******************************************

  Description:  Node setup

*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.5     Yasperzee   12'22          Add HC-SRO4 Ultrasonic Distance Sensor     
  Version 0.4     Yasperzee   12'22          Cleaning and refactoring
  Version 0.3     Yasperzee   12'22          IR Thermometer support added
  Version 0.2     Yasperzee   12'22          Sensor model and type added
  Version 0.1     Yasperzee   12'22          Imported to platformio

  ----------------------------------------------------------------------------*/
  #include <Arduino.h>
  //#include "ESP8266WiFi.h"
  //#include <ESP8266WebServer.h>  

//void do_setup(void);

//************ Configurations ************

  // Select Sensor type
  //#define SENSOR_RPM
  //#define SENSOR_IR_THERMOMETER
  #define SENSOR_ULTRASONIC_DISTANCE
  
  // Tachometer settings
  #define WINGS 1  // PulsesPerRevolution

   // IR Thermometer settings
  #define EMISSIVITY 1.00  

  // Increment number for each node
  //#define NODE_ID_STR "Node_00001"  // this is obsolete, use ESP.getChipId(); 

  #define APP_SW_VERSION "zee_esp_http_0.2.0_dev"
  #define HW_VERSION "zee_esp_tacometer_0.1.0_demo"
  #define NODEMCU_STR "ESP-01s"
  #define NODE_FUNCTION "Tacometer" // e.g. Tacometer, IR-Thermometer,  Weather, Environment, Handheld, ...
  #define SENSOR_MODEL_STR "None" // e.g. Olimex SNS-IR-3-8

  #define RPM_PIN 2 // ESP-01
  //#define I2C_SCL_PIN 2 //ESP01 
  //#define I2C_SDA_PIN 0 //ESP01 
  //#define DEBUG_PIN 0 // e.g. for external LED
  //#define RPM_PIN 5 // D1 in NodeMcu
 

  #define BAUDRATE 115200
  #define WIFI_RETRY_TIME 1000

  #define HTTP_PORT 80
  #define HTTP_REST_PORT 8080

  #define EEPROM_SIZE 512

#endif //SETUP_H