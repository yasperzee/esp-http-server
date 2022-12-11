/***************************read_sensors.cpp************************************

    Description:    Read sensor and return Value
                    Supported sensors: Optic RPM sensors 

*******************************************************************************/
/*------------------------------------------------------------------------------
    Version 0.7     Yasperzee   12'22   Add BMP280 & BME280 Sensors
    Version 0.6     Yasperzee   12'22   Add HC-SRO4 Ultrasonic Distance Sensor  
    Version 0.5     Yasperzee   12'22   Cleaning and refactoring
    Version 0.4     Yasperzee   11'22   IR TEMPERATURE sensor support
    Version 0.3     Yasperzee   11'22   Weather stuff removed
    Version 0.2     Yasperzee   11'22   RPM meter support added
    Version 0.1     Yasperzee   06'19   DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/
#if defined(SENSOR_DHT11) || defined(SENSOR_DHT22)
#include <read_DHT_sensors.h>
#endif

#include "read_sensors.h"
#include "eeprom.h"
#include <EEPROM.h>
#include <Adafruit_MLX90614.h>
#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>

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
//#endif
 //#if defined SENSOR_TACOMETER
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
//#endif
 //#if defined SENSOR_IR_THERMOMETER
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
//#endif // SENSOR_IR_THERMOMETER

//if defined SENSOR_ULTRASONIC_DISTANCE
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

/*
  if(values.distanceCm >= minDist && values.distanceCm <= maxDist) {
    digitalWrite(relayPin, LOW);
    delay(lightsOnDelay);
    digitalWrite(relayPin, HIGH);
    }
*/
  return values;
  }
//#endif

#ifdef SENSOR_BMP280
Values ReadSensors::read_bmp280()
    {
    //char bmp280_status;
    //Values values;

    Adafruit_BMP280 bmp280;

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!bmp280.begin(BMP280_ADDR, BMP280_CHIPID))
        {// continue anyway but show valeus as "-999.99" (ERROR_VALUE)
        Serial.println("BMP280 init FAIL!!");
        values.temperature = ERROR_VALUE;
        values.pressure = ERROR_VALUE;
        values.altitude = ERROR_VALUE;
        values.fail_count ++;
        }
    else
        {
        values.temperature = bmp280.readTemperature();
        values.pressure = (bmp280.readPressure()/100); // mBar
        values.altitude = bmp280.readAltitude(1013.25);
        //values.altitude = bmp280.readAltitude();
        #ifdef TRACE_INFO
        Serial.print("BMP280: Temperature: ");
        Serial.println(values.temperature);
        Serial.print("BMP280: Pressure   : ");
        Serial.println(values.pressure);
        Serial.print("BMP280: Altitude   : ");
        Serial.println(values.altitude);
        #endif
        #ifdef NODE_FEATURE_AMBIENT_LIGHT
            pinMode(ALS_PIN, INPUT);
            values.als = analogRead(ALS_PIN); //Read light levelM
            //float reading = analogRead(ALS_PIN); //Read light level
            //float square_ratio = reading / 1023.0; //Get percent of maximum value (1023)
            //square_ratio = pow(square_ratio, 2.0);
            //values.als = reading;
        #endif
        #ifdef TRACE_INFO
        Serial.print("temt6000: ");
        Serial.print(values.als);
        Serial.print("\n");
        #endif
        }
    return values;
    } //read_bmp280
#endif

#ifdef SENSOR_BME280
Values ReadSensors::read_bme280()
    {
    //BME280_ADDRESS                (0x77)
    //BME280_ADDRESS_ALTERNATE      (0x76)

    Adafruit_BME280 bme280;
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    //Wire.setClock(100000);

    if(!bme280.begin(BME280_ADDRESS))
        {// continue anyway but show valeus as "-999.99" (ERROR_VALUE)
        Serial.println("BME280 init FAIL!!");
        values.temperature  = ERROR_VALUE;
        values.pressure     = ERROR_VALUE;
        values.altitude     = ERROR_VALUE;
        values.humidity     = ERROR_VALUE;
        values.fail_count ++;
        }
    else
        {
        values.temperature = bme280.readTemperature();
        values.humidity = bme280.readHumidity();
        values.pressure = bme280.readPressure();
        //values.altitude = bme280.readAltitude();
        #ifdef TRACE_INFO
        Serial.print("BME280: Temperature: ");
        Serial.println(values.temperature);
        Serial.print("BME280: Humidity: ");
        Serial.println(values.humidity);
        Serial.print("BME280: Pressure: ");
        Serial.println(values.pressure);
        Serial.print("BME280: Altitude: ");
        Serial.println(values.altitude);
        #endif

        #ifdef NODE_FEATURE_AMBIENT_LIGHT
            pinMode(ALS_PIN, INPUT);
            float reading = analogRead(ALS_PIN); //Read light level
            //float square_ratio = reading / 1023.0; //Get percent of maximum value (1023)
            //square_ratio = pow(square_ratio, 2.0);
            values.als = reading;
          #ifdef TRACE_INFO
            Serial.print("ALS: ");
            Serial.println(values.als);
          #endif
        #endif
        }
    return values;
} //read_bme280
#endif

  //IRAM_ATTR void ReadSensors:: isr() {
IRAM_ATTR void isr() {
    rev++;
    //digitalWrite (DEBUG_PIN, LOW);  
    //delay(2);  // Some Delay
    //digitalWrite (DEBUG_PIN, HIGH); 
    }
