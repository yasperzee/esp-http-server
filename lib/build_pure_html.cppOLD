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