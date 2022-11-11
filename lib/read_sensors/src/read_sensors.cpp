
/***************************read_dht_sensor.cpp**********************************

    Description:    Read sensors and return Values struct
                    Supported sensors: DHT11 & DTH22,

*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.1     Yasperzee   6'19  DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/
#include <read_sensors.h>



DHT dht(DHT_PIN, DHT_TYPE);

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
    //values.humidity = (int)H;
    //values.temperature = roundf(T * 100) / 100; // 2 decimals 
    //Serial.println("humid: %d "), values.humidity;
    //Serial.println("temp: %d "), values.temperature;

    // Check if any reads failed and exit early (to try again).
    if (isnan(H) || isnan(T))
        {
        Serial.println(F("Failed to read DHT sensor!"));
        values.temperature = ERROR_VALUE;
        values.humidity = ERROR_VALUE;
      	}

    return values;
    }
