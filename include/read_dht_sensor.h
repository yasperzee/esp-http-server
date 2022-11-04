#ifndef READ_DHT_SENSOR_H
#define READ_DHT_SENSOR_H

#include <Adafruit_Sensor.h>
#include "DHT.h"

// values from DHT read_dht_sensor
struct Values{
    float temperature;
    float humidity;
    };

    // Select DHT sensor in use, select "SENSOR_STR" also !
    //#define DHT_TYPE      DHT11
    //String SENSOR_STR =   "DHT-11";
    #define DHT_TYPE 	  DHT22
    String SENSOR_STR = "DHT-22";
    #define DHT_PIN 	2 // ESP-01 gpio 2

    // constants
    const float ErrorValue = -999.9;

    // Initialize DHT sensor.
    DHT dht(DHT_PIN, DHT_TYPE);




/*
  ************************** .h *************************

  Description:  
  ************************************************************/
  /*-----------------------------------------------------------
    Version 0.1     Yasperzee   11'22  

  -----------------------------------------------------------
*/

#endif // READ_DHT_SENSOR_H