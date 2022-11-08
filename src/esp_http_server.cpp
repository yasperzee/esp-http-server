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
 
---------------------------------------------------------------------------------*/

// includes
#include "ArduinoJson.h"
#include "ssid.h"  // SSID and PASS strings for local network
#include "setup.h"
#include "read_dht_sensor.h" 
#include <aREST.h> // For Rest

// Functions
String build_json_getdata_html(void);
String build_json_getinfo_html(void);
void serverRoutingRest();
void handleNotFoundRest();
void getNodeData();
void getNodeInfo();

// Set web server port number
ESP8266WebServer server(HTTP_PORT);  //Define server object

void setup()
    {
    Serial.begin(BAUDRATE);
    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
        {
        delay(WIFI_RETRY_TIME);
        Serial.print(".");
        }  
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
    //Serial.println("Node is " + NODEMCU_STR);
    //Serial.println("Sensor is " + SENSOR_STR);
     
    //Associate handler function to web requests
    serverRoutingRest();
    server.onNotFound(handleNotFoundRest);        // When a Rest client requests an unknown URI (i.e. something other than "/"), call function "handleNotFoundRest"
    server.begin();

    } // setup

void loop()
    {
    //Handle Client requests
    server.handleClient();
    } // loop

void serverRoutingRest() {
    server.on("/", HTTP_GET, []() 
        {
        server.send(200, F("text/html"), F("Welcome to the REST  IOT Web Server"));
        server.on(F("/nodeData"), HTTP_GET, getNodeData);
        server.on(F("/nodeInfo"), HTTP_GET, getNodeInfo); // there can be several "on"
       // server.on(F("/nodeSetting"), HTTP_GET, getNodeSettings); // there can be several "on"
        });  
}

void getNodeData() {
    String temp = build_json_getdata_html();
    server.send(200, "text/json", temp);
}

void getNodeInfo() {
    String temp = build_json_getinfo_html();
    server.send(200, "text/json", temp);
}

// Manage not found URL ( Rest)
void handleNotFoundRest() {
  String message = "File Not Found\n\n";
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






 /* Moved to read_dht_sensor.cpp
Values read_dht_sensor(void)
    {
    float T,H;
    Values values;

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    H = dht.readHumidity();
    // Read temperature as Celsius (the default)
    T = dht.readTemperature();
    //values.humidity = H;
    //values.temperature = T;
    values.humidity = (int)H;
    values.temperature = roundf(T * 100) / 100; // 2 decimals 
    //Serial.println("humid: %d "), values.humidity;
    //Serial.println("temp: %d "), values.temperature;

    // Check if any reads failed and exit early (to try again).
    if (isnan(H) || isnan(T))
        {
        Serial.println(F("Failed to read DHT sensor!"));
        values.temperature = ErrorValue;
        values.humidity = ErrorValue;
      	}
    return values;
    }
*/

/* Moved to build_json_docs.cpp
String build_json_getdata_html(void)
    {

    String webpage;

    values = read_dht_sensor();

    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Temp"] = values.temperature;
    root["Humid"] = values.humidity;
    root["RPM"] = "n/a";
    //root["RSSI"] = WiFi.RSSI();
    //root["VCC"] = ESP.getVcc();
    //root["INFO"] = "ESP-01s";

    //Store JSON in String variable  
    serializeJson(root, webpage);

    //Serial.println("webpage: ");
    //Serial.println(webpage);

    return webpage;
    }
*/

/* Moved to build_json_docs.cpp
String build_json_getinfo_html(void)
    {

    String webpage;

    values = read_dht_sensor();

    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    //root["Temp"] = values.temperature;
    //root["Humid"] = values.humidity;
    root["RSSI"] = WiFi.RSSI();
    root["VCC"] = ESP.getVcc();
    root["CHIPID"] = ESP.getChipId();
    root["NODETYPE"] = "ESP-01s";

    //Store JSON in String variable  
    serializeJson(root, webpage);

    //Serial.println("webpage: ");
    //Serial.println(webpage);

    return webpage;
    }
*/