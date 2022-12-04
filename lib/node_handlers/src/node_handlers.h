#ifndef NODE_HANDLERS_H
#define NODE_HANDLERS_H

/*************************** node_handlers.h ************************************
  
  Description:  

*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.1     Yasperzee   12'22     Cleaning and refactoring
 
 #TODO:
------------------------------------------------------------------------------*/
//#include "setup.h"
//#include <ESP8266WebServer.h> 

  void handle_iot_rest_client();
  void restServerRoutingRest();
  void handleNotFoundRest();
  void getNodeSettings();
  void putNodeSettings();
  void getNodeDebug();
  void getNodeData();
  void getNodeInfo();
 
#endif // NODE_HANDLERS_H