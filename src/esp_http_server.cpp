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
  
    Version 0.4     11'22     Yasperzee     Rest added

    Version 0.3     11'22     Yasperzee     Json added

    Version 0.2     11'22     Yasperzee     Imported to platformio
  
    Version 0.1     11'22     Yasperzee     Baseline, pure HTML
    
    Version 1.1b    4'19      Yasperzee     Something. . .

    // jSon
    // https://tutorial.eyehunts.com/html/display-formatted-json-in-html-example-code/
    // https://arduinojson.org/v6/example/http-server/
    // https://stackoverflow.com/questions/51748948/how-to-give-a-name-to-json-object
    // https://arduinojson.org/v6/api/jsondocument/createnestedobject/

    // Rest
    https://www.mischianti.org/2020/05/16/how-to-create-a-rest-server-on-esp8266-and-esp32-startup-part-1/
    https://github.com/sidoh/rich_http_server/blob/master/examples/SimpleRestServer/SimpleRestServer.ino
 
---------------------------------------------------------------------------------*/

// includes
#include "ssid.h"  // SSID and PASS strings for local network
#include "setup.h"
#include "read_dht_sensor.h" 
#include "ArduinoJson.h"
//#include <RichHttpServer.h> // For Rest


Values values;

// Functions
String build_json_html(void);
Values read_dht_sensor(void);
void serverRoutingRest();
void handleNotFoundRest();
void getNodeData();
//void handleRootRest();

// Set web server port number to 80
ESP8266WebServer server(HTTP_PORT);  //Define server object
// configure the REST port and instatiate the server
//ESP8266WebServer server(HTTP_REST_PORT);

// Initialize DHT sensor.
DHT dht(DHT_PIN, DHT_TYPE);

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
        // REST
      
      

    dht.begin();    

    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("RSSI: %d dBm"), WiFi.RSSI();
    Serial.println("Vcc: %d V"), ESP.getVcc();

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

String build_json_html(void)
    {

    String webpage;

    values = read_dht_sensor();

    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Temp"] = values.temperature;
    root["Humid"] = values.humidity;
    root["RSSI"] = WiFi.RSSI();
    root["VCC"] = ESP.getVcc();
    root["INFO"] = "ESP-01s";
   //Serial.println("root: ");

    //Store JSON in String variable  
    serializeJson(root, webpage);

    //Serial.println("webpage: ");
    //Serial.println(webpage);

    return webpage;
    }

void serverRoutingRest() {
    server.on("/", HTTP_GET, []() 
        {
        server.send(200, F("text/html"), F("Welcome to the REST  IOT Web Server"));
        server.on(F("/nodeData"), HTTP_GET, getNodeData);
        //server.on(F("/settings"), HTTP_GET, getSettings); // there can be several "on"
        });  
}

void getNodeData() {
    String temp = build_json_html();
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


/*
void handleRootRest() {
    //String temp = build_json_html();
    //server.send(200, "text/json", temp);
    server.send(200, F("text/json"), F("Welcome to the REST Web Server, handleRootRest"));
    //getNodeData();
}
*/