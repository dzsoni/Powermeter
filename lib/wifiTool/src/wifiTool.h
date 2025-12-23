/**************************************************************
   wifiTool is a library for the ESP 8266&32/Arduino platform
   SPIFFS oriented AsyncWebServer based wifi configuration tool.
   https://github.com/oferzv/wifiTool
   
   Built by Ofer Zvik (https://github.com/oferzv)
   And Tal Ofer (https://github.com/talofer99)
   Licensed under MIT license
 **************************************************************/

#ifndef WIFITOOL_H
#define WIFITOOL_H

#include <Arduino.h>
#include <DNSServer.h>
#include <vector>
#include <utility>
#include "SimpleJsonWriter.h"

#ifdef ESP32
#include <FS.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
//#include <ESP8266mDNS.h>
#endif

#include <ESPAsyncWebServer.h>
#include <SimpleJsonParser.h>
#include <..\..\src\struct_hardwares.h>
#include <MQTTMediator.h>
#include <WifiManager.h>


#include "definitions.h"

#define  DEBUG_WIFITOOL 

#ifdef DEBUG_WIFITOOL
#define _WIFITOOL_PP(a) Serial.print(a);
#define _WIFITOOL_PL(a) Serial.println(a);
#else
#define _WIFITOOL_PP(a)
#define _WIFITOOL_PL(a)
#endif

struct struct_hardwares;


class WifiTool
{
public:
  WifiTool(struct_hardwares& sh);
  ~WifiTool();
  void process();
  void begin();

private:
  void setUpSoftAP();
  unsigned long         _restartsystem;
  SimpleJsonParser      _sjsonp;
  struct_hardwares&     _sh;
  File                  _fsUploadFile;

  std::unique_ptr<DNSServer> dnsServer;
  


  //void  updateUpload();
  void  handleFileList(AsyncWebServerRequest *request);
  void  handleFileDelete(AsyncWebServerRequest *request);
  void  getWifiScanJson(AsyncWebServerRequest *request);
  void  handleSaveSecretJson(AsyncWebServerRequest *request);
  void  handleSaveMqtt(AsyncWebServerRequest* request);
  void  handleGetMqttjson(AsyncWebServerRequest *request);
  int   getRSSIasQuality(int RSSI);
  void  handleUpload(AsyncWebServerRequest *request, String filename, String redirect, size_t index, uint8_t *data, size_t len, bool final);
  void  handleGetDeviceNames(AsyncWebServerRequest *request);
  void  handleGetfilteredCommands(AsyncWebServerRequest *request);
  void  handleGetDeviceUsernames(AsyncWebServerRequest *request);
  void  handleSaveCommandFilter(AsyncWebServerRequest *request);
  void  handleFileDownload(AsyncWebServerRequest *request);
  void  handleGetVersion(AsyncWebServerRequest *request);
  void  handleGetCheckPZEM(AsyncWebServerRequest *request);
  void  handleGetUsedSerials(AsyncWebServerRequest *request);
  void  handleSavePZEMaddress(AsyncWebServerRequest *request);
};

#endif /* WIFITOOL_H */
