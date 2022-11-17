
/***************************read_dht_sensor.cpp**********************************

    Description:    Read sensors and return Values struct
                    Supported sensors: DHT11 & DTH22,

*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.2     Yasperzee   11'22   RPM meter support added
    Version 0.1     Yasperzee   06'19   DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/
#include <read_sensors.h>

 // RPM stuff
//float rev;
float rev;
float rpm;
int   oldtime;
int   newtime;

void IRAM_ATTR isr()
{
    //Serial.print("rev in isr:");
    rev++;
    //Serial.println(rev);
}

float get_rpm()
  {
  //noInterrupts();
  detachInterrupt(RPM_PIN); //detaches the interrupt
  newtime=millis()-oldtime; //finds the time 
  //int debug_oldtime= oldtime;

  int wings= 1; // no of wings of rotating object, for disc object use 1 with white tape on one side
  float RPMnew = rev/wings; 
  rpm=(RPMnew/newtime)*60000; //calculates rpm
  oldtime=millis(); //saves the current time
 
  //values.rpm=rpm;
   // int status = digitalRead(RPM_PIN);
int newtime_secs =newtime/1000;
    Serial.println("\n___TACHOMETER___");
    Serial.print("rev: ");
    Serial.println(rev);
    Serial.print(rpm);
    Serial.println(" RPM");
    Serial.print("Time in secs: ");
    Serial.println(newtime_secs);

   // Serial.println(" ");
    //Serial.print("values.RPM: ");
    //Serial.println(values.rpm);
   
    //Serial.println(debug_oldtime);
    //Serial.print("oldtime: ");
    //Serial.println(oldtime);
    
    //Serial.print("debug_oldtime: ");
    //Serial.println(debug_oldtime);
    //Serial.print("newtime: ");
    //Serial.println(newtime);

    rev=0;
    //interrupts();
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);
    //delay(1000);
    return rpm;

}

/**/
//DHT dht(DHT_PIN, DHT_TYPE);

Values read_dht_sensor(void)
    {
    float T, H;
    Values values;

    DHT dht(DHT_PIN, DHT_TYPE);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    H = dht.readHumidity();
    // Read temperature as Celsius (the default)
    T = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(H) || isnan(T))
        {
        Serial.println(F("Failed to read DHT sensor!"));
        values.temperature = ERROR_VALUE;
        values.humidity = ERROR_VALUE;
      	}
    else
        { 
        values.humidity = H;
        values.temperature = T;
        //values.humidity = (int)H;
        //values.temperature = roundf(T * 100) / 100; // 2 decimals 
        Serial.print("humid: ");
        Serial.println(values.humidity);
        Serial.print("temp: ");
        Serial.println(values.temperature);
    }
    return values;
    }
/**/