/***************************read_sensors.cpp*********************************

    Description:    Read sensors and return Value
                    Supported sensors: Optic RPM sensors 

*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.3     Yasperzee   11'22   Weather stuff removed
    Version 0.2     Yasperzee   11'22   RPM meter support added
    Version 0.1     Yasperzee   06'19   DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/
#include "read_sensors.h"
#include <Arduino.h>

 // RPM stuff
float rev;
float rpm;
int   oldtime;
int   newtime;
float revTime;
int wings;

void IRAM_ATTR isr() {
    //detachInterrupt(RPM_PIN); //detaches the interrupt
    rev++;
    //Serial.print("rev: (isr) ");
    //Serial.println(rev);
    //digitalWrite (DebugIOpin, HIGH); // Making PIN High.
    //digitalWrite (DebugIOpin, LOW);  // Making PN LOW.
    //delay(10);              // Some Delay
    //digitalWrite (DebugIOpin, HIGH); // Making PIN High.
    //delay (5); //ms
    //attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);
    }

float get_rpm() {
    detachInterrupt(RPM_PIN); //detaches the interrupt
    newtime=millis()-oldtime; //finds the time 
    wings= 2; // PulsesPerRevolution, for Olimex SNS-IR-3-8 set to 2, it sends pulse on FALLING and RAISING edges
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

    rev=0;
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);
    return rpm;
    }

