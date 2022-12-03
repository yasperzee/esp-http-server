/***************************read_sensors.cpp*********************************

    Description:    Read sensor and return Value
                    Supported sensors: Optic RPM sensors 

*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.4     Yasperzee   11'22   IR TEMPERATURE sensor support
    Version 0.3     Yasperzee   11'22   Weather stuff removed
    Version 0.2     Yasperzee   11'22   RPM meter support added
    Version 0.1     Yasperzee   06'19   DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/
#include "read_sensors.h"
#include <Adafruit_MLX90614.h>



Adafruit_MLX90614 mlx = Adafruit_MLX90614();

extern Values values;

// RPM stuff
float rev;
float rpm;
int   oldtime;
int   newtime;
float revTime;
int wings= 1; // PulsesPerRevolution, for Olimex SNS-IR-3-8 set to 2.

// IR Temperature meter stuff
//====== CHANGE THIS ========
double new_emissivity =1.00;
//===========================

void IRAM_ATTR isr() {
    //detachInterrupt(RPM_PIN); //detaches the interrupt
    rev++;
    //Serial.print("rev: (isr) ");
    //Serial.println(rev);
    //digitalWrite (DEBUG_PIN, LOW);  // Making PN LOW.
    //delay(2);  // Some Delay
    //digitalWrite (DEBUG_PIN, HIGH); // Making PIN High.

    //attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);
    }

float get_rpm() {
    detachInterrupt(RPM_PIN); //detaches the interrupt
    newtime=millis()-oldtime; //finds the time 
    wings= 1; // PulsesPerRevolution, for Olimex SNS-IR-3-8 set to 2, it sends pulse on FALLING and RAISING edges
    float RPMnew = rev/wings; 
    rpm=(RPMnew/newtime)*60000; //calculates rpm
    oldtime=millis(); //saves the current time
    int newtime_secs =newtime/1000;
    Serial.println("\n___TACHOMETER___");
    Serial.print("rev: ");
    Serial.println(rev);
    Serial.print(rpm);
    Serial.println(" RPM");
    Serial.print("Time in secs: ");
    Serial.println(newtime_secs);
    Serial.print("wings: ");
    Serial.println(wings);

    rev=0;
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);
    return rpm;
    }

void get_ir_temperature()
    {
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

        double currentE = mlx.readEmissivity();
        Serial.print("Setting emissivity = "); 
        Serial.println(new_emissivity);

        // set new emissivity if not same as current one 
        if(new_emissivity != currentE) {
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
    Serial.print("");
    }
