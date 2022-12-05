/***************************setup.cpp ******************************************

  Description:  

*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.1     Yasperzee   12'22     Cleaning and refactoring
 
  #TODO:
------------------------------------------------------------------------------*/
#include "setup.h"
#include "debug.h"
#include "eeprom.h"
#include <WiFiManager.h> 

extern int wings;
extern void set_callbacks();

//EEPROM stuff
extern localEeprom  eeprom_c;
uint16 reboots_eeprom_address = 0; // address to save reboots
int reboots_eeprom_length = sizeof(reboots_eeprom_address); // size of data to save eeprom
uint8 wings_eeprom_address = sizeof(reboots_eeprom_address)+1; // address to save wings
int wings_eeprom_length = sizeof(wings_eeprom_address); // size of data to save eeprom

void do_setup() {

#ifdef SENSOR_RPM // Infrared sensor for Tacometer
  pinMode(RPM_PIN, INPUT_PULLUP); 
#elif defined SENSOR_IR_TEMPERATURE  // IR Thermometer
  //Do something if any...
#endif
  
  Serial.begin(BAUDRATE);
  EEPROM.begin(EEPROM_SIZE);
  //clear_eeprom();

  // read reboots count from EEPROM, increment and write back
  int reboots = eeprom_c.read_eeprom(reboots_eeprom_address);
  reboots++;
  eeprom_c.write_eeprom(reboots_eeprom_address, reboots);
  eeprom_c.write_eeprom(wings_eeprom_address, wings);
 
  // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.
  WiFiManager wifiManager;

  //reset saved WifiManager settings, for debugging  
  //wifiManager.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  bool result;
  // result = wifiManager.autoConnect(); // auto generated AP name from chipid
  result = wifiManager.autoConnect("AutoConnectAP"); // anonymous ap
  //result = wifiManager.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!result) {
    Serial.println("Failed to connect WiFi");
    // ESP.restart();
    } 
  else { 
    Serial.println("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());
    }

  set_callbacks();

#ifdef SENSOR_RPM
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);; 
#elif defined SENSOR_IR_TEMPERATURE
  //Do something if any...
#endif

  printInfo();
}
