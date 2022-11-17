#ifndef READ_SENSORS_H
#define READ_SENSORS_H

/*
  ************************** read_sensors.h *************************

  Description:  
  ************************************************************/
  /*-----------------------------------------------------------

    Version 0.2     11'22     Yasperzee     RPM support

    Version 0.1     11'22     Yasperzee     Imported to platformio

  -----------------------------------------------------------*/
#include <Adafruit_Sensor.h>
#include <DHT.h>

 #define DHT_PIN 2 // ESP01 uses gpio 2 for DHT11 / DHT22 sensor
 #define RPM_PIN 0 
 #define DHT_TYPE DHT11
 //#define DHT_TYPE DHT22
 #define SENSOR_STR "Tacometer" // Weather, Environment ...


    // constants
    const int ERROR_VALUE = -999;

// values from DHT read_dht_sensor
   struct Values{
      float temperature;
      float humidity;
      float rpm;
      };

Values read_dht_sensor(void);
void IRAM_ATTR isr(void);
float get_rpm(void);

#endif // READ_SENSORS_H