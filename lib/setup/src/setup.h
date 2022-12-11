#ifndef SETUP_H
#define SETUP_H

/*************************** setup.h *******************************************

  Description:  Node setup

*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.6     Yasperzee   12'22          Add BMP280 & BME280 Sensors
  Version 0.5     Yasperzee   12'22          Add HC-SRO4 Ultrasonic Distance Sensor     
  Version 0.4     Yasperzee   12'22          Cleaning and refactoring
  Version 0.3     Yasperzee   12'22          IR Thermometer support added
  Version 0.2     Yasperzee   12'22          Sensor model and type added
  Version 0.1     Yasperzee   12'22          Imported to platformio

  ----------------------------------------------------------------------------*/
  #include <Arduino.h>

//************ Configurations ************
 #define TRACE_INFO
  // Select Sensor type
 #define SENSOR_BME280
  //#define SENSOR_BMP280
  //#define SENSOR_DHT22
  //#define SENSOR_TACOMETER
  //#define SENSOR_IR_THERMOMETER
  //#define SENSOR_ULTRASONIC_DISTANCE
  
  // Tachometer settings
  #define WINGS 1  // PulsesPerRevolution

   // IR Thermometer settings
  #define EMISSIVITY 1.00  

  // Increment number for each node
  //#define NODE_ID_STR "Node_00001"  // this is obsolete, use ESP.getChipId(); 

  #define APP_SW_VERSION "zee_esp_http_0.2.0_dev"
  #define HW_VERSION "zee_esp_tacometer_0.1.0_demo"
  #define NODEMCU_STR "ESP-01s"
  #define NODE_FUNCTION "Temp&Humid" // e.g. Ultrasonic-distance, Tacometer, IR-Thermometer,  Weather, Environment, Handheld, ...
  #define SENSOR_MODEL_STR "DHT22" // e.g. Olimex SNS-IR-3-8

  
 
  

  //#define BMP280_ADDR BMP280_ADDRESS //(0x77) SDO = HIGH
#define BMP280_ADDR BMP280_ADDRESS_ALT  //(0x76) SDO = LOW
// #define ALTITUDE 119.0 // Altitude of Tampere-Pirkkala airport, Finland. In meters
#define ALTITUDE 129.0 // Altitude of Kalkunvuori, Tampere Finland. In meters
 
  #define BAUDRATE 115200
  #define WIFI_RETRY_TIME 1000
  
  #define HTTP_PORT 80
  #define HTTP_REST_PORT 8080

#endif //SETUP_H