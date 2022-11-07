

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
//Select string for Node, used for information and debug
String NODEMCU_STR  =  "ESP-01";
// Increment number for each node
String NODE_ID_STR  =  "Node-00001";

// *****************************************************************************

// defines
#define PORT            80
#define BAUDRATE        115200
#define WIFI_RETRY_TIME 1000 //ms

#define HTTP_PORT 80
#define HTTP_REST_PORT 8080

// variables
//String getValuesState   = "off";

#endif //SETUP_H