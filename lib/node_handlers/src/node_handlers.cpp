
#include "node_handlers.h"


ESP8266WebServer rest_server(HTTP_PORT);

void getNodeData() {
  String temp = build_json_getdata_html();
  rest_server.send(200, "text/json", temp);
  }
 
  void getNodeInfo() {
  String temp = build_json_getinfo_html();
  rest_server.send(200, "text/json", temp);
  }

void getNodeDebug() {
  String temp = build_json_getDebug_html();
  rest_server.send(200, "text/json", temp);
  }
  
void getNodeSettings() {
  String temp = build_json_getSettings_html();
  rest_server.send(200, "text/json", temp);
  }

  void putNodeSettings() {
  String temp = build_json_putSettings_html();
  rest_server.send(200, "text/json", temp);
  }

  // Manage not found URL ( Rest)
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

  void set_callbacks() {
  //Associate handler functions to web requests
  restServerRoutingRest();
  rest_server.on(F("/nodeData"), HTTP_GET, getNodeData);
  rest_server.on(F("/nodeInfo"), HTTP_GET, getNodeInfo); 
  rest_server.on(F("/nodeDebug"), HTTP_GET, getNodeDebug); 
  rest_server.on(F("/nodeSettings"), HTTP_GET, getNodeSettings); 
  //rest_server.on(F("/nodeSettings"), HTTP_PUT, putNodeSettings);
  rest_server.onNotFound(handleNotFoundRest);        // When a Rest client requests an unknown URI (i.e. something other than "/"), call function "handleNotFoundRest"
  rest_server.begin(); 
}