/**************************** esp_http_server.cpp ***********************

  Description:  Read temperature & humidity from DHT11 & DHT22 sensor.
                ESP-01 acts as webserver.
                Builds webpage with temperature and humidity values.
                Builds webpage with some information about NodeMCU and Sensor.

  Components:   - ESP-01 esp8266 NodeMcu
                - DHT11 or DHT22 temperature and humidity sensor

  Librarys:     - uhttps://github.com/esp8266/Arduino
                - https://github.com/adafruit/DHT-sensor-library
                - https://github.com/adafruit/Adafruit_Sensor

  IDE & tools:  - VSCode, Platformio

  References:   -
*******************************************************************************/

/*------------------------------------------------------------------------------
    
    Version 0.7     11'22     Yasperzee     /nodeDebug added
    
    Version 0.6     11'22     Yasperzee     RPM meter support added
    
    Version 0.5     11'22     Yasperzee     /nodeData  and /nodeInfo separated

    Version 0.4     11'22     Yasperzee     Rest added

    Version 0.3     11'22     Yasperzee     Json added

    Version 0.2     11'22     Yasperzee     Imported to platformio
  
    Version 0.1     11'22     Yasperzee     Baseline, pure HTML
    
    Version 1.1b    4'19      Yasperzee     Something. . .

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
--------------------------------------------------------------------------------------------*/

// includes
//#include "ArduinoJson.h"
#include "ssid.h"  // SSID and PASS strings for local network
#include "setup.h"
#include "read_sensors.h" 
#include "build_json_docs.h"
//#include "rest_callbacks.h"
#include "build_light_html.h"

// Functions
void printInfo();
void handle_web_client();
void serverRoutingRest();
void handleNotFoundRest();
void getNodeData();
void getNodeInfo();
float get_rpm();
void getNodeDebug();
void getNodeSettings();

u32 reboots=  0; // save to EEPROM

// Set web server port number
ESP8266WebServer server(HTTP_PORT);  //Define server object

// IR Infrared sensor
  //pinMode(RPM_PIN, INPUT_PULLUP); 
  //pinMode(RPM_PIN, INPUT); 

void setup() {
  
  reboots++; // to EEPROM

  Serial.begin(BAUDRATE);
  // Connect to Wi-Fi network with SSID and password
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED){
      delay(WIFI_RETRY_TIME);
      Serial.print(".");
      }  

  printInfo();
  
  //Associate handler function to web requests
  serverRoutingRest();
  server.on(F("/nodeData"), HTTP_GET, getNodeData);
  server.on(F("/nodeInfo"), HTTP_GET, getNodeInfo); 
  server.on(F("/nodeDebug"), HTTP_GET, getNodeDebug); 
  server.on(F("/nodeSettings"), HTTP_GET, getNodeSettings); 
  //server.on(F("/nodeSetup"), HTTP_PUT, putNodeSetup); 
  server.onNotFound(handleNotFoundRest);        // When a Rest client requests an unknown URI (i.e. something other than "/"), call function "handleNotFoundRest"
  server.begin(); 

  // Infrared sensor for Tacometer
  pinMode(RPM_PIN, INPUT_PULLUP); // use external pull-up resistor
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);
  } // setup

void loop() {    
  //get_rpm();
  //read_dht_sensor();

  //Handle Client requests
  handle_web_client();
  } // loop

void handle_web_client(void) {
  server.handleClient();
  }

void getNodeData() {
  String temp = build_json_getdata_html();
  server.send(200, "text/json", temp);
  }

void getNodeInfo() {
  String temp = build_json_getinfo_html();
  server.send(200, "text/json", temp);
  }

void getNodeDebug() {
  String temp = build_json_getDebug_html();
  server.send(200, "text/json", temp);
  }
  
void getNodeSettings() {
  String temp = build_json_getSettings_html();
  server.send(200, "text/json", temp);
  }
  
// Manage not found URL ( Rest)
void handleNotFoundRest() {
  String message = "File Not Found\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
  server.send(404, "text/plain", message);
  }

void serverRoutingRest() {
  server.on("/", HTTP_GET, []() {
    // Send webpage
    String webpage;
    webpage = build_light_html();
    server.send(200, ("text/html"), (webpage));
    });  
  }

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
    Serial.print("Sensor is ");
    Serial.println(SENSOR_STR);
    }

    