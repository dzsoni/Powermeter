#include <Arduino.h>
#include "wifiTool.h"
#include "struct_hardwares.h"
#include <SimpleJsonParser.h>
#include <TaskScheduler.h>

#include "task_defs.h"
#include "mqtt_callbacks.h"
#include "board_def.h"
#include "version.h"

#include <BasicOTA.h>
#include "MQTTMediator.h"

#include "CommandCenter.h"
#include "MQTT_Command.h"
#include "Serial_Command.h"
#include "WifiManager.h"
#include "ESPAsyncWebServer.h"

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#ifdef ARDUINO_ARCH_ESP8266
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
#endif

//Forward declarations
String getVersion();
//-------------------------------------------------------------------------------
WifiManager         wifimanager;
MQTTMediator        mqttmediator;
TupleCoreFactory    tuplecorefactory;
CommandCenter       commandcenter;
MQTT_Command        mqttcommand(&mqttmediator, commandcenter, MQTTCOMMAND_SETTINGS_JSON);
Serial_Command      serialcommand(commandcenter);
AsyncWebServer      webserver(80);
Scheduler           taskManager;
struct_hardwares    sh(webserver, tuplecorefactory, commandcenter, mqttcommand, &mqttmediator, mqttmediator, wifimanager);
WifiTool            wifiTool(sh);

// Global Task declarations - needed by mqtt_callbacks.cpp
Task task1(1000, TASK_FOREVER, &t1Cb_secTimer, &taskManager, false);
Task task2(30000, TASK_FOREVER, &t2Cb_writeMQTT, &taskManager, false);
Task mqttReconnectTimer(10000, TASK_FOREVER, &connectToMqtt, &taskManager, false);

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println(F("\nSystem started."));
  Serial.print(String(F("Version: ")) + getVersion());
  Serial.print(F(" Compiled: "));
  Serial.print(String(__DATE__) + String(F("-")));
  Serial.println(__TIME__);
  Serial.printf("Cpu Frequency: %u MHz\n", ESP.getCpuFreqMHz());

  if (!SPIFFS.begin())
    {
        // Serious problem
        Serial.println(F("SPIFFS Mount failed."));
    }
  
  BasicOTA.begin(&webserver);
  Serial.println(F("BasicOTA started."));

  //wifi init
  //wifimanager.setPathToSecretjson("/secrets.json");
  wifimanager.begin();//using  with the default path  "/secrets.json"
  //webserver init
  wifiTool.begin();
    
  //LED init
  pinMode(FLASH_LED, OUTPUT);
  digitalWrite(FLASH_LED, LOW);

  //MQTT init
    #ifdef ARDUINO_ARCH_ESP32
    WiFi.onEvent(onWifiConnect,  ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(onWifiDisconnect, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    #elif defined(ARDUINO_ARCH_ESP8266)
    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
    #endif
  
  // Tasks are now declared globally above
  
}

void loop() {
  wifiTool.process();
  taskManager.execute();
  commandcenter.process();
  serialcommand.processSerialInput();
  wifimanager.wifiAutoReconnect();
}
