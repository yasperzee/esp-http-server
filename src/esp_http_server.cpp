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
    
--------------------------------------------------------------------------------------------*/
#include "setup.h"
#include "debug.h"
#include "read_sensors.h" 
#include "build_json_docs.h"
#include "eeprom.h"
#include <WiFiManager.h> 
#include "node_handlers.h"

//extern ESP8266WebServer rest_server(HTTP_PORT);

extern int wings;

// Globals
Values values;

//EEPROM stuff
uint16 reboots_eeprom_address = 0; // address to save reboots
int reboots_eeprom_length = sizeof(reboots_eeprom_address); // size of data to save eeprom
uint8 wings_eeprom_address = sizeof(reboots_eeprom_address)+1; // address to save wings
int wings_eeprom_length = sizeof(wings_eeprom_address); // size of data to save eeprom

void setup() {
#ifdef SENSOR_RPM
  // Infrared sensor for Tacometer
  pinMode(RPM_PIN, INPUT_PULLUP); 
#elif defined SENSOR_IR_TEMPERATURE
  //Do something if any...
#endif
  
  Serial.begin(BAUDRATE);
  EEPROM.begin(EEPROM_SIZE);
  //clear_eeprom();

  // read reboots count from EEPROM, increment and write back
  int reboots = read_eeprom(reboots_eeprom_address);
  reboots++;
  write_eeprom(reboots_eeprom_address, reboots);
  Serial.print("Reboots from EEPROM: ");
  Serial.println(reboots);
  write_eeprom(wings_eeprom_address, wings);
  Serial.print("Wings from EEPROM: ");
  Serial.println(wings);

  // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.
  WiFiManager wifiManager;

  //reset saved WifiManager settings, for debugging  
  //wifiManager.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  bool result;
  // result = wifiManager.autoConnect(); // auto generated AP name from chipid
  result = wifiManager.autoConnect("AutoConnectAP"); // anonymous ap
  //result = wifiManager.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!result) {
    Serial.println("Failed to connect WiFi");
    // ESP.restart();
    } 
  else { 
    Serial.println("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());
    }

  set_callbacks();

#ifdef SENSOR_RPM
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);; 
#elif defined SENSOR_IR_TEMPERATURE
  //Do something if any...
#endif

  printInfo();
  } // setup

void loop() {    
 
  handle_iot_rest_client();

  } // loop


    