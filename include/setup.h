

#ifndef SETUP_H
#define SETUP_H


#include <Arduino.h>

#include "ESP8266WiFi.h"



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

// variables
String getValuesState   = "off";







#endif //SETUP_H