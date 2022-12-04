/***************************read_sensors.cpp************************************

    Description:    Read sensor and return Value
                    Supported sensors: Optic RPM sensors 

*******************************************************************************/

/*------------------------------------------------------------------------------
    Version 0.5     Yasperzee   12'22   Cleaning and refactoring
    Version 0.4     Yasperzee   11'22   IR TEMPERATURE sensor support
    Version 0.3     Yasperzee   11'22   Weather stuff removed
    Version 0.2     Yasperzee   11'22   RPM meter support added
    Version 0.1     Yasperzee   06'19   DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/
#include "read_sensors.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Values values;

// RPM stuff
float rev;
float rpm;
int   oldtime;
int   newtime;
float revTime;
int wings= 1; // PulsesPerRevolution, for Olimex SNS-IR-3-8 set to 2.
//IRAM_ATTR void  isr();

// IR Thermometer stuff
//====== CHANGE THIS ========
double new_emissivity =1.00;
//===========================

//IRAM_ATTR void ReadSensors:: isr() {
IRAM_ATTR void isr() {
    rev++;
    //digitalWrite (DEBUG_PIN, LOW);  
    //delay(2);  // Some Delay
    //digitalWrite (DEBUG_PIN, HIGH); 
    }

Values ReadSensors::get_rpm() {
//float get_rpm() {
    detachInterrupt(RPM_PIN);
    newtime=millis()-oldtime; //finds the time 
    wings= 1; // PulsesPerRevolution, for Olimex SNS-IR-3-8 set to 2, it sends pulse on FALLING and RAISING edges
    float RPMnew = rev/wings; 
    values.rpm =(RPMnew/newtime)*60000; //calculates rpm
    oldtime=millis(); //saves the current time
    int newtime_secs =newtime/1000;

    Serial.println("\n___TACHOMETER___");
    Serial.print("rev: ");
    Serial.println(rev);
    Serial.print(values.rpm);
    Serial.println(" RPM");
    Serial.print("Time in secs: ");
    Serial.println(newtime_secs);
    Serial.print("wings: ");
    Serial.println(wings);
    rev=0;
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);

    //return rpm;
    return values;
    }

Values ReadSensors::get_ir_temperature() {
//void get_ir_temperature() {
    Serial.println("\nAdafruit MLX90614 Emissivity Setter.");

    // init ir_temp sensor
    if (!mlx.begin()) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");
        //while (1);
        }
    else {
        // read current emissivity
        Serial.print("Current emissivity = "); 
        Serial.println(mlx.readEmissivity());

        values.emissivity= mlx.readEmissivity();
         
        Serial.print("Setting emissivity = "); 
        Serial.println(new_emissivity);

        // set new emissivity if not same as current one 
        if(new_emissivity != values.emissivity) {
            mlx.writeEmissivity(new_emissivity); // this does the 0x0000 erase write
   
            Serial.print("New emissivity = "); 
            Serial.println(mlx.readEmissivity());

        Serial.println("DONE. Restart the module.");
            }
        }            

    Serial.print("MLX90614 ObjectTemperature in C: ");
    values.ir_object_temp = mlx.readObjectTempC();
    Serial.println(values.ir_object_temp);

    Serial.print("MLX90614 Ambient Temperature in C: ");
    values.ir_ambient_temp = mlx.readObjectTempC();
    Serial.println(values.ir_ambient_temp);
    Serial.println();

    return values;
    }
