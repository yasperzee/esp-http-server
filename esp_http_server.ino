/**************************** weather_esp01_dht_http.ino ***********************

  Description:  Read temperature & humidity from DHT11 & DHT22 sensor.
                ESP-01 acts as webserver.
                Builds webpage with temperature and humidity values.
                Builds webpage with some information about NodeMCU and Sensor.

  Components:   - ESP-01 esp8266 NodeMcu
                - DHT11 or DHT22 temperature and humidity sensor

  Librarys:     - uhttps://github.com/esp8266/Arduino
                - https://github.com/adafruit/DHT-sensor-library
                - https://github.com/adafruit/Adafruit_Sensor

  IDE & tools:  - Arduino IDE 1.8.8, UBUNTU 18.04 LTS

  References:   -
*******************************************************************************/

/*------------------------------------------------------------------------------
  
    Version 0.1     11'22     Yasperzee     Baseline, pure HTML
    
    Version 1.1b    4'19      Yasperzee     Something. . .
 

------------------------------------------------------------------------------*/

// includes
#include "ssid.h"  // SSID and PASS strings for local network
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "ESP8266WiFi.h"
#include "DHT.h"

//****** Configurations ********************************************************
//Select string for Node, used for information and debug
String NODEMCU_STR  =  "ESP-01";

// Increment number for each node
String NODE_ID_STR  =  "Node-00001";

// Select DHT sensor in use, select "SENSOR_STR" also !
//#define DHT_TYPE      DHT11
//String SENSOR_STR =   "DHT-11";
#define DHT_TYPE 	  DHT22
String SENSOR_STR = "DHT-22";

#define DHT_PIN 	2 // ESP-01 gpio 2

// *****************************************************************************

// defines
#define PORT            80
#define BAUDRATE        115200
#define WIFI_RETRY_TIME 1000 //ms

// constants
const float ErrorValue = -999.9;
// variables
String getValuesState   = "off";

// Variable to store the HTTP request
String header;

// values from DHT read_dht_sensor
struct Values
    {
    float temperature;
    float humidity;
    };

// Functions
Values read_dht_sensor(void);
String build_pure_html(void);

// Set web server port number to 80
WiFiServer server(PORT);
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
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    dht.begin();
    Serial.println("Node is " + NODEMCU_STR);
    Serial.println("Sensor is " + SENSOR_STR);
    } // setup

void loop()
    {
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client)
        { // If a new client connects
        Serial.println("New Client.");
        String currentLine = ""; // make a String to hold incoming data from the client
        while (client.connected())
            { // loop while the client's connected
            if (client.available())
                { // if there's bytes to read from the client,
                char c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n')
                    { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                        {
                        Serial.println("Send response");
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        //client.println("Connection: close");
                        client.println();

                        // GET values
                        if (header.indexOf("GET /TH/data") >= 0)
                            {
                             getValuesState = "on";
                            }
                        else if (header.indexOf("GET /TH/info") >= 0)
                            {
                            getValuesState= "off";
                            }
                        else
                            {
                            getValuesState= "error";
                            }
                        // Send the HTML web page
                        String tmp = build_pure_html();
                        //Serial.println(tmp);
                        client.println(tmp);
                        //client.println("Connection closed.");
                        // Break out of the while loop
                        break;
                        } // if (currentLine.length() == 0)
                    else
                        { // if you got a newline, then clear currentLine
                        currentLine = "";
                        }
                    } // if (c == '\n')
                else if (c != '\r')
                    {  // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                    }
                } // if (client.available())
            } // while (client.connected())
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
        }  // if(client)
    } // loop

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
