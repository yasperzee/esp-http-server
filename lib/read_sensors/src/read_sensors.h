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
  
#include <Adafruit_Sensor.h>

#define RPM_PIN 2 // ESP-01
//#define RPM_PIN 5 // D1 in NodeMcu
#define SENSOR_STR "Tacometer" // Weather, Environment ...

// constants
//const int ERROR_VALUE = -999;

void IRAM_ATTR isr(void);
float get_rpm(void);

#endif // READ_SENSORS_H