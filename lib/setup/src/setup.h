#ifndef SETUP_H
#define SETUP_H
/*************************** setup.h *************************

  Description:  Node setup

************************************************************/

/*-----------------------------------------------------------

  Version 0.1     11'22     Yasperzee     Imported to platformio

  -----------------------------------------------------------*/

  #include <Arduino.h>
  #include "ESP8266WiFi.h"
  #include <ESP8266WebServer.h>  

//****** Configurations ********************************************************

  // Increment number for each node
  //#define NODE_ID_STR "Node_00001"  // use ESP.getChipId(); 

  #define APP_SW_VERSION "zee_esp_http_0.1.0_dev"
  #define NODEMCU_STR "ESP-01s"


  #define BAUDRATE        115200
  #define WIFI_RETRY_TIME 1000

  #define HTTP_PORT 80
  #define HTTP_REST_PORT 8080

  #define EEPROM_SIZE 512

//ESP8266WebServer rest_server(HTTP_PORT);

#endif //SETUP_H