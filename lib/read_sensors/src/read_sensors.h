#ifndef READ_SENSORS_H
#define READ_SENSORS_H

/*************************** read_sensors.h ************************************
 
  Description:  
  
*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.6     Yasperzee   12'22   Add HC-SRO4 Ultrasonic Distance Sensor  
  Version 0.5     Yasperzee   12'22     Cleaning and refactoring
  Version 0.4     Yasperzee   11'22     IR TEMPERATURE sensor support
  Version 0.3     Yasperzee   11'22     Weather stuff removed
  Version 0.2     Yasperzee   11'22     RPM support
  Version 0.1     Yasperzee   11'22     Imported to platformio

#TODO:
------------------------------------------------------------------------------*/
//#include <Arduino.h>
#include "setup.h"

// constants
const int ERROR_VALUE = -999.99;

// values for sensor readings
struct Values {
    //double temperature  = ERROR_VALUE;
    //double pressure     = ERROR_VALUE;
    //double altitude     = ERROR_VALUE;
    //double humidity     = ERROR_VALUE;
    //double als          = ERROR_VALUE;
    //double vcc_batt     = ERROR_VALUE;
    
    double rpm            = ERROR_VALUE;
    int wings             = 1;

    double emissivity     = EMISSIVITY;
    double ir_object_temp = ERROR_VALUE;
    double ir_ambient_temp= ERROR_VALUE;

    double distanceCm     = ERROR_VALUE;
    };

class ReadSensors {
  public:
    Values get_rpm(void);
    Values get_ir_temperature();
    // IRAM_ATTR void isr();
    void set_emissivity();
    Values ReadUltrasonicSensor(); 
   private:
};

// Functions
void IRAM_ATTR isr(void); // ToDo: move to Class

#endif // READ_SENSORS_H


