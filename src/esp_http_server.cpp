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
  
    Version 0.3     11'22     Yasperzee     Json added

    Version 0.2     11'22     Yasperzee     Imported to platformio
  
    Version 0.1     11'22     Yasperzee     Baseline, pure HTML
    
    Version 1.1b    4'19      Yasperzee     Something. . .

    // https://tutorial.eyehunts.com/html/display-formatted-json-in-html-example-code/
    // https://arduinojson.org/v6/example/http-server/
    // https://stackoverflow.com/questions/51748948/how-to-give-a-name-to-json-object
    // https://arduinojson.org/v6/api/jsondocument/createnestedobject/
 
------------------------------------------------------------------------------*/

// includes
#include "ssid.h"  // SSID and PASS strings for local network
#include "setup.h"
#include "read_dht_sensor.h" 

//#include "esp8266webserver.h"
#include <ESP8266WebServer.h>   // Include the WebServer library

#include "ArduinoJson.h"


// Variable to store the HTTP request
String header;
Values values;

// Functions
String build_json_html(void);
Values read_dht_sensor(void);
void handleRoot();
void handleNotFound();

//String build_pure_html(void);

// Set web server port number to 80
//WiFiServer server(PORT);
ESP8266WebServer server(80);  //Define server object
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
    dht.begin();    
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("RSSI: %d dBm"), WiFi.RSSI();
    Serial.println("Vcc: %d V"), ESP.getVcc();
     
    //Associate handler function to web requests

    server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
    server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

    server.begin();

    Serial.println("Node is " + NODEMCU_STR);
    Serial.println("Sensor is " + SENSOR_STR);


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

    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<500> doc;
    // Create the root object
    JsonObject root = doc.createNestedObject("FullInfo");
    root["Temp"] = values.temperature;
    root["Humid"] = values.humidity;
    root["RSSI"] = WiFi.RSSI();
    root["VCC"] = ESP.getVcc();
    root["INFO"] = "ESP-01s"; 

    //Store JSON in String variable  
    serializeJson(doc, webpage);

    Serial.println("webpage: ");
    Serial.println(webpage);

    return webpage;
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
    values.humidity = H;
    values.temperature = T;
    // Check if any reads failed and exit early (to try again).
    if (isnan(H) || isnan(T))
        {
        Serial.println(F("Failed to read DHT sensor!"));
        values.temperature = ErrorValue;
        values.humidity = ErrorValue;
      	}
    return values;
    }

void handleRoot() {
    String temp = build_json_html();
    server.send(200, "text/plain", temp); 
    //server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
    Serial.println("HTTP status 200 (Ok)");
    
}

void handleNotFound(){
    server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
    Serial.println("HTTP status 404 (Not found)");
}
