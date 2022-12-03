#ifndef READ_SENSORS_H
#define READ_SENSORS_H

/*************************** read_sensors.h ************************************
  Description:  
*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.3     11'22     Yasperzee       Weather stuff removed
  Version 0.2     11'22     Yasperzee       RPM support
  Version 0.1     11'22     Yasperzee       Imported to platformio

------------------------------------------------------------------------------*/
#include <Arduino.h>
//#include <Adafruit_Sensor.h>
#include "setup.h"

//#define RPM_PIN 2 // ESP-01
//#define RPM_PIN 5 // D1 in NodeMcu
//#define SENSOR_STR "Tacometer" // Weather, Environment ...

// constants
const int ERROR_VALUE = -999.99;

// values for sensor readings
struct Values
    {
    double temperature  = ERROR_VALUE;
    double pressure     = ERROR_VALUE;
    double altitude     = ERROR_VALUE;
    double humidity     = ERROR_VALUE;
    double als          = ERROR_VALUE;
    double vcc_batt     = ERROR_VALUE;
    double rpm          = ERROR_VALUE;
    double ir_object_temp     = ERROR_VALUE;
    double ir_ambient_temp      = ERROR_VALUE;
    int fail_count      = 0;
    };

// Functions
void IRAM_ATTR isr(void);
float get_rpm(void);
//void mlxSensor();
void get_ir_temperature();

#endif // READ_SENSORS_H