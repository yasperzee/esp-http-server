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

<<<<<<< HEAD
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

=======
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
>>>>>>> Last-Arduino-IDE-version

#endif // READ_DHT_SENSOR_H