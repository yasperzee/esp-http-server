/***************************read_sensors.cpp************************************

    Description:    Read sensor and return Value
                    Supported sensors: Optic RPM sensors 

*******************************************************************************/
/*------------------------------------------------------------------------------
    Version 0.6     Yasperzee   12'22   Add HC-SRO4 Ultrasonic Distance Sensor  
    Version 0.5     Yasperzee   12'22   Cleaning and refactoring
    Version 0.4     Yasperzee   11'22   IR TEMPERATURE sensor support
    Version 0.3     Yasperzee   11'22   Weather stuff removed
    Version 0.2     Yasperzee   11'22   RPM meter support added
    Version 0.1     Yasperzee   06'19   DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/
#include "read_sensors.h"
#include "eeprom.h"
#include <EEPROM.h>
#include <Adafruit_MLX90614.h>
#include "ESP8266WiFi.h"
#include <Arduino.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
extern localEeprom  eeprom_c;
Values values;

// RPM stuff
extern uint8 wings_eeprom_address;
float rev;
float rpm;
int   oldtime;
int   newtime;
float revTime;

// IR Thermometer stuff
extern uint8 emissivity_eeprom_address;
void set_emissivity();

//#ifdef SENSOR_ULTRASONIC_DISTANCE
  const int trigPin = 12; //D6
  const int echoPin = 14; // D5

  //define sound velocity in cm/uS
  #define SOUND_VELOCITY 0.034
  #define CM_TO_INCH 0.393701
  long duration;
  float distanceCm;
  
  //Values ReadUltrasonicSensor(); 

//#endif

Values ReadSensors::get_rpm() {
    detachInterrupt(RPM_PIN);
    newtime=millis()-oldtime; //finds the time 
    
    // Read ppr (PulsesPerRevolution) from EEPROM
    int wings= eeprom_c.read_eeprom(wings_eeprom_address);
    if(wings <= 0) wings = 1; // Just in case if EEPROM is NOK. . .
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

    return values;
    }

Values ReadSensors::get_ir_temperature() {
    // init ir_temp sensor
    if (!mlx.begin()) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");
        //while (1);
        }
    else {
        set_emissivity();
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

void ReadSensors::set_emissivity() {
  //set new emissivity if not same as current one
  Serial.println("\nAdafruit MLX90614 Emissivity Setter.");
  // read current emissivity
  double curr_emiss;
   
  curr_emiss = mlx.readEmissivity();
  if (isnan(curr_emiss)) { // try again
    delay (100);
    curr_emiss = mlx.readEmissivity();
    }
  Serial.print("Current emissivity = "); 
  Serial.println(curr_emiss);
 
  //previous_emissivity 
  Serial.print("values.emissivity = "); 
  Serial.println(values.emissivity);

  if(fabs(curr_emiss - values.emissivity)) {
    mlx.writeEmissivity(EMISSIVITY); // this does the 0x0000 erase write
    //Write emissivity for IR Thermometr to EEPROM
    eeprom_c.write_eeprom(emissivity_eeprom_address, EMISSIVITY);
    values.emissivity= mlx.readEmissivity();
   // delay(100);
    Serial.print("New emissivity: "); 
    Serial.println(mlx.readEmissivity());
    Serial.println("DONE. Restart the module.");
    }
  }

Values ReadSensors::ReadUltrasonicSensor() {
  Serial.println("\nUltrasonic Sensor HC-SR04");
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  values.distanceCm = distanceCm;
  // Prints the distance on the Serial Monitor
  // Serial.print("duration: ");
  // Serial.println(duration);
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.println();
  //Serial.print("Distance (inch): ");
  //Serial.println(distanceInch);
  
  // delay(3000);
  return values;
  }

  //IRAM_ATTR void ReadSensors:: isr() {
IRAM_ATTR void isr() {
    rev++;
    //digitalWrite (DEBUG_PIN, LOW);  
    //delay(2);  // Some Delay
    //digitalWrite (DEBUG_PIN, HIGH); 
    }