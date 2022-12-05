/**************************** esp_http_server.cpp ******************************

Description:  Read RPM

Components:   - ESP-01 esp8266 NodeMcu
              - Optic RPM sensor

Librarys:     -

References:   
  // ESP
    // https://arduino-esp8266.readthedocs.io/en/latest/libraries.html

  // WiFi
    // https://www.arduino.cc/reference/en/libraries/wifi/

  // jSon
    // https://tutorial.eyehunts.com/html/display-formatted-json-in-html-example-code/
    // https://arduinojson.org/v6/example/http-server/
    // https://stackoverflow.com/questions/51748948/how-to-give-a-name-to-json-object
    // https://arduinojson.org/v6/api/jsondocument/createnestedobject/

  // Rest
    // https://www.mischianti.org/2020/05/16/how-to-create-a-rest-server-on-esp8266-and-esp32-startup-part-1/
    // https://github.com/sidoh/rich_http_server/blob/master/examples/SimpleRestServer/SimpleRestServer.ino
    // https://www.arduino.cc/reference/en/libraries/arest/
    // https://github.com/brunoluiz/arduino-restserver
 
  // RPM
    // https://www.circuitschools.com/diy-tachometer-using-arduino-esp8266-esp32-to-measure-accurate-rpm-with-ir-sensor/
*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 1.3     12'22     Yasperzee     Cleaning and refactoring
  Version 1.2     12'22     Yasperzee     IR Thermometer support added
  Version 1.0     11'22     Yasperzee     Wifi manager added
  Version 0.9     11'22     Yasperzee     EEPROM support added
  Version 0.8     11'22     Yasperzee     Weather stuff removed
  Version 0.7     11'22     Yasperzee     /nodeDebug added  
  Version 0.6     11'22     Yasperzee     RPM meter support added
  Version 0.5     11'22     Yasperzee     /nodeData  and /nodeInfo separated
  Version 0.4     11'22     Yasperzee     Rest added
  Version 0.3     11'22     Yasperzee     Json added
  Version 0.2     11'22     Yasperzee     Imported to platformio
  Version 0.1     11'22     Yasperzee     Baseline, pure HTML
  Version 1.1b    4'19      Yasperzee     Something. . .
    
#TODO:
--------------------------------------------------------------------------------------------*/
#include "setup.h"
#include "debug.h"
#include "read_sensors.h" 
#include "build_json_docs.h"
#include "eeprom.h"
#include <WiFiManager.h> 
#include "node_handlers.h"

extern void do_setup();
extern void handle_iot_rest_client();

void setup() {
  do_setup();
  } // setup

void loop() {    
  handle_iot_rest_client();
  } // loop


    