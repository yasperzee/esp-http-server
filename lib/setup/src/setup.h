

#ifndef SETUP_H
#define SETUP_H
/*
  ************************** setup.h *************************

  Description:  
  ************************************************************/
  /*-----------------------------------------------------------
    Version 0.1     11'22     Yasperzee     Imported to platformio

  -----------------------------------------------------------
*/
#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>   // Include the WebServer library

//****** Configurations ********************************************************

// Increment number for each node
//#define NODE_ID_STR "Node_00001"  // use ESP.getChipId(); 
#define APP_SW_VERSION "zee_0.0.1"
#define SENSOR_STR "DHT11"
#define NODEMCU_STR "ESP-01s"


// defines
#define PORT            80
#define BAUDRATE        115200
#define WIFI_RETRY_TIME 1000 //ms

// variables
#define BAUDRATE        115200
#define WIFI_RETRY_TIME 1000 //ms

#define HTTP_PORT 80
#define HTTP_REST_PORT 8080

#endif //SETUP_H