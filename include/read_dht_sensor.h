#ifndef READ_DHT_SENSOR_H
#define READ_DHT_SENSOR_H

/*
  ************************** read_dht_sensor.h *************************

  Description:  
  ************************************************************/
  /*-----------------------------------------------------------
    Version 0.1     11'22     Yasperzee     Imported to platformio

  -----------------------------------------------------------
*/

#include <Adafruit_Sensor.h>
#include "DHT.h"


 #define DHT_PIN 2 // ESP01 and ESP12 uses gpio 2 for DHT11 / DHT22 sensor
 #define DHT_TYPE DHT11
    //#define SENSOR_STR "DHT22"
    //#define DHT_TYPE DHT22

    // constants
    const int ERROR_VALUE = -999;

// values from DHT read_dht_sensor
struct Values{
   int temperature;
   int humidity;
   };

Values read_dht_sensor(void);

#endif // READ_DHT_SENSOR_H