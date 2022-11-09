

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
<<<<<<< HEAD
=======
#include <ESP8266WebServer.h>   // Include the WebServer library
>>>>>>> Last-Arduino-IDE-version

//****** Configurations ********************************************************
//Select string for Node, used for information and debug
String NODEMCU_STR  =  "ESP-01";
// Increment number for each node
String NODE_ID_STR  =  "Node-00001";

// *****************************************************************************

// defines
<<<<<<< HEAD
#define PORT            80
#define BAUDRATE        115200
#define WIFI_RETRY_TIME 1000 //ms

// variables
String getValuesState   = "off";
=======
//#define PORT            80
#define BAUDRATE        115200
#define WIFI_RETRY_TIME 1000 //ms

#define HTTP_PORT 80
#define HTTP_REST_PORT 8080

/*
ESP.getChipId() returns the ESP8266 chip ID as a 32-bit integer.
ESP.getCoreVersion() returns a String containing the core version.
ESP.getSdkVersion() returns the SDK version as a char.
*/

>>>>>>> Last-Arduino-IDE-version

#endif //SETUP_H