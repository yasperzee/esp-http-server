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
//String build_pure_html(void);

// Set web server port number to 80
//WiFiServer server(PORT);
ESP8266WebServer server(80);  //Define server object
// Initialize DHT sensor.
DHT dht(DHT_PIN, DHT_TYPE);

//Handles http request 
void handleRoot() {
  
    String webPage;
    webPage = build_json_html();
    server.send(200, "text/html", webPage);
}

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
     
    server.on("/", handleRoot);      //Associate handler function to web requests
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
    //webpage  = "{\"products\":";
    root["Temp"] = values.temperature;
    root["Humid"] = values.humidity;
    root["RSSI"] = WiFi.RSSI();
    root["VCC"] = ESP.getVcc();
    root["INFO"] = "ESP-01s"; 

    //Store JSON in String variable  
    serializeJson(doc, webpage);
    //webpage  += "}";

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

/*
String build_pure_html(void)
    {
    Values values;
    String webpage;

    webpage  = "<!DOCTYPE html><html>";
    webpage += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    webpage += "<link rel=\"icon\" href=\"data:,\">";
    // CSS to style the on/off buttons
    // Feel free to change the background-color and font-size attributes to fit your preferences
    webpage += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    webpage += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
    webpage += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    webpage += ".button2 {background-color: #77878A;}</style></head>";
    // Web Page Heading

    // Display temperature and pressure values
    if (getValuesState=="off")
        {
        webpage += "<p>Node information </p>";
        webpage += "<p><a href=\"/TH/data\"><button class=\"button\">Get data</button></a></p>"; // Next state
        webpage += "<info>";
        webpage +=  "Info: ";
        webpage +=  NODE_ID_STR;
        webpage += ": ";
        webpage +=  NODEMCU_STR;
        webpage += " / ";
        webpage +=  SENSOR_STR;
        webpage += "</info>";
        }
    else if (getValuesState=="on")
        {
        values = read_dht_sensor();
        webpage += "<p>Measurements </p>";
        webpage += "<p><a href=\"/TH/info\"><button class=\"button button2\">Get Node info</button></a></p>"; // Next state
        // Print temperature and humidity values here
        webpage += "<data>";
        webpage += "Temperature: ";
        webpage += (values.temperature);
        webpage += " C";
        webpage += "<br/>";
        webpage += "Humidity: ";
        webpage += (values.humidity);
        webpage += " %";
        webpage += "</data>";
        }
    else
        {
        webpage += "<p>Unsupported request </p>";
        webpage += "<p><a href=\"/TH/info\"><button class=\"button button2\">Get Node info</button></a></p>"; // Next state
        webpage += "<error>";
        webpage += "Unknown request. ";
        webpage += "</error>";
        }
    
    webpage += "</body></html>";
    // The HTTP response ends with another blank line
    webpage += "";
    return (webpage);
    } // build_pure_html

*/
