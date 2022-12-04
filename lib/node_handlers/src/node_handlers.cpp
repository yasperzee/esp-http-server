/*************************** node_handlers.cpp *********************************

  Description:  

*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.1     Yasperzee   12'22     Cleaning and refactoring
 
  #TODO:
------------------------------------------------------------------------------*/
#include "node_handlers.h"
#include "ArduinoJson.h"
#include "setup.h"
#include "build_json_docs.h"
#include "build_light_html.h"
//#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h> 

// static ESP8266WebServer rest_server(HTTP_PORT);
// ESP8266WebServer rest_server(HTTP_PORT);
static ESP8266WebServer rest_server(HTTP_PORT);

BuildJsonDocs_C  build_json_docs_c;

void set_callbacks() { 
  // Associate handler functions to web requests
  restServerRoutingRest();
  rest_server.on(F("/nodeData"), HTTP_GET, getNodeData);
  rest_server.on(F("/nodeInfo"), HTTP_GET, getNodeInfo); 
  rest_server.on(F("/nodeDebug"), HTTP_GET, getNodeDebug); 
  rest_server.on(F("/nodeSettings"), HTTP_GET, getNodeSettings); 
  //rest_server.on(F("/nodeSettings"), HTTP_PUT, putNodeSettings);
  rest_server.onNotFound(handleNotFoundRest);        // When a Rest client requests an unknown URI (i.e. something other than "/"), call function "handleNotFoundRest"
  rest_server.begin(); 
}

void getNodeData() {
  String temp = build_json_docs_c.build_json_getdata_html();
  rest_server.send(200, "text/json", temp);
  }
 
  void getNodeInfo() {
  rest_server.send(200, "text/json", build_json_docs_c.build_json_getinfo_html());
  }

void getNodeDebug() {
  rest_server.send(200, "text/json", build_json_docs_c.build_json_getDebug_html());
  }
  
void getNodeSettings() {
  rest_server.send(200, "text/json", build_json_docs_c.build_json_getSettings_html());
  }

void putNodeSettings() {
  rest_server.send(200, "text/json", build_json_docs_c.build_json_putSettings_html());
  }

void handleNotFoundRest() {
  String message = "File Not Found\n";
  message += "URI: ";
  message += rest_server.uri();
  message += "\nMethod: ";
  message += (rest_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += rest_server.args();
  message += "\n";
  for (uint8_t i = 0; i < rest_server.args(); i++) {
    message += " " + rest_server.argName(i) + ": " + rest_server.arg(i) + "\n";
    }
  rest_server.send(404, "text/plain", message);
  }

void restServerRoutingRest() {
  rest_server.on("/", HTTP_GET, []() {
    // Send webpage
    String webpage;
    webpage = build_light_html();
    rest_server.send(200, ("text/html"), (webpage));
    });  
  }

  void handle_iot_rest_client() {
    rest_server.handleClient();
  }