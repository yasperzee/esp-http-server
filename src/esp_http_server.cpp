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

// includes
#include "setup.h"
#include "read_sensors.h" 
#include "build_json_docs.h"

#include "eeprom.h"
#include "node_handlers.cpp"

#include <WiFiManager.h> 

int reboots_eeprom_address = 0; // address to save reboots
int sensor_eeprom_address = 1; //  Sensor string to EEPROM
extern int wings;
// Functions
void printInfo();
void handle_rest_client();
void restServerRoutingRest();
void handleNotFoundRest();
void getNodeData();
void getNodeInfo();
float get_rpm();
void getNodeDebug();
void getNodeSettings();
void putNodeSettings();

void setup() {
  
  EEPROM.begin(EEPROM_SIZE);

  Serial.begin(BAUDRATE);


    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    //WiFiManager
    WiFiManager wifiManager;

    //reset saved settings, for debugging
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
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }


/*
  // Connect to Wi-Fi network with SSID and password
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(WIFI_RETRY_TIME);
      Serial.print(".");
      }  
*/
  //clear_eeprom();

  // read reboots count from EEPROM, increment and write back
  int reboots = read_eeprom(reboots_eeprom_address);
  reboots++;
  write_eeprom(reboots_eeprom_address, reboots);
  Serial.print("Reboots: ");
  Serial.println(reboots);

  printInfo();
  
  //Associate handler function to web requests
  restServerRoutingRest();
  rest_server.on(F("/nodeData"), HTTP_GET, getNodeData);
  rest_server.on(F("/nodeInfo"), HTTP_GET, getNodeInfo); 
  rest_server.on(F("/nodeDebug"), HTTP_GET, getNodeDebug); 
  rest_server.on(F("/nodeSettings"), HTTP_GET, getNodeSettings); 
  //rest_server.on(F("/nodeSettings"), HTTP_PUT, putNodeSettings);
  rest_server.onNotFound(handleNotFoundRest);        // When a Rest client requests an unknown URI (i.e. something other than "/"), call function "handleNotFoundRest"
  rest_server.begin(); 

  // Infrared sensor for Tacometer
  pinMode(RPM_PIN, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);
  } // setup

void loop() {    
 
  handle_rest_client();
  } // loop

void printInfo(void) {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: "), WiFi.RSSI();
    Serial.println(WiFi.RSSI());
    Serial.print("MAC: ");  
    Serial.println(WiFi.macAddress()); 
    Serial.print("Vcc: ");  
    Serial.println(ESP.getVcc());
    Serial.print("Core version: "); 
    Serial.println(ESP.getCoreVersion()); //returns a String containing the core version.
    Serial.print("Chip ID: "); 
    Serial.println(ESP.getChipId()); //returns the ESP8266 chip ID as a 32-bit intege
    Serial.print("SDK version: ");
    Serial.println(ESP.getSdkVersion());ESP.getSdkVersion(); //returns the SDK version as a char.
    Serial.print("Node is ");
    Serial.println(NODEMCU_STR);
    Serial.print("APP_SW_VERSION is ");
    Serial.println(APP_SW_VERSION);
     Serial.print("HW_VERSION is ");
    Serial.println(HW_VERSION);
    Serial.print("Sensor is ");
    Serial.println(SENSOR_STR);
    Serial.print("Wings is ");
    Serial.println(wings);
    }




/*
void clear_eeprom() {
 
  Serial.print("");
  Serial.print("EEPROM.length: ");
  Serial.println(EEPROM.length());
  Serial.print("");

  for (size_t addr = 0 ; addr < EEPROM.length() ; addr++) {
    int value = EEPROM.read(addr);
    Serial.print(value);
    delay(10);
    }
  Serial.print("");

  for (size_t i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
    //delay(10);
    }
  EEPROM.commit();

  Serial.print("");

  for (size_t addr = 0 ; addr < EEPROM.length() ; addr++) {
    int value = EEPROM.read(addr);
    Serial.print(value);    
    delay(10);
    } 
  }
*/
/*
int read_eeprom(int address) {

  int value = EEPROM.read(address);
  return value;
  }
*/
/*
void write_eeprom( int addr, int value) {

  EEPROM.write(addr, value);
  delay(10);
  Serial.print("");
  if (EEPROM.commit()) {
    Serial.println("EEPROM successfully committed");
    } 
  else {
    Serial.println("ERROR! EEPROM commit failed");
    }
  delay(10);
  }  

void getNodeSettings(void) {
  rest_server.handleClient();
  }
*/
/*
void getNodeData() {
  String temp = build_json_getdata_html();
  rest_server.send(200, "text/json", temp);
  }

void getNodeInfo() {
  String temp = build_json_getinfo_html();
  rest_server.send(200, "text/json", temp);
  }

void getNodeDebug() {
  String temp = build_json_getDebug_html();
  rest_server.send(200, "text/json", temp);
  }
  
void getNodeSettings() {
  String temp = build_json_getSettings_html();
  rest_server.send(200, "text/json", temp);
  }

  void putNodeSettings() {
  String temp = build_json_putSettings_html();
  rest_server.send(200, "text/json", temp);
  }

// Manage not found URL ( Rest)
void handleNotFoundRest() {
  String message = "File Not Found\n";
  message += "URI: ";
  message += rest_server.uri();
  message += "\nMethod: ";
  message += (rest_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += rest_server.args();
  message += "\n";
  for (uint8_t i = 0; i < rest_server.args(); i++) {
    message += " " + rest_server.argName(i) + ": " + rest_server.arg(i) + "\n";
    }
  rest_server.send(404, "text/plain", message);
  }

void restServerRoutingRest() {
  rest_server.on("/", HTTP_GET, []() {
    // Send webpage
    String webpage;
    webpage = build_light_html();
    rest_server.send(200, ("text/html"), (webpage));
    });  
  }
*/

    