#ifndef BUILD_JSON_DOCS_H
#define BUILD_JSON_DOCS_H


/*************************build_json_docs.h************************************
    Description:    
*******************************************************************************/

/* ------------ Version history ------------------------------------------------
    Version 0.1     Yasperzee   11'22 

#TODO:
------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "ArduinoJson.h"
#include <read_sensors.h>
#include "ESP8266WiFi.h"
#include "setup.h"

String build_json_getdata_html(void);
String build_json_getinfo_html(void);
String build_json_getDebug_html(void);
String build_json_getSettings_html(void);

#endif // BUILD_JSON_DOCS_H