#include <Arduino.h>
#include <esp_task_wdt.h>
#include <SPIFFS.h>
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

#include <ArduinoJson.h>
#include <MycilaPZEM.h>

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

// Convert global objects to pointers - initialized in setup()
MQTTMediator*        mqttmediator = nullptr;
TupleCoreFactory*    tuplecorefactory = nullptr;
CommandCenter*       commandcenter = nullptr;
MQTT_Command*        mqttcommand = nullptr;
Serial_Command*      serialcommand = nullptr;
AsyncWebServer*      webserver = nullptr;
Scheduler*           taskManager = nullptr;
WifiManager*         wifimanager = nullptr;
struct_hardwares*    sh = nullptr;
WifiTool*            wifiTool = nullptr;

Mycila::PZEM*     pzem1=nullptr; // 0x01
Mycila::PZEM*     pzem2=nullptr; // 0x02
Mycila::PZEM*     pzem3=nullptr; // 0x03

Mycila::PZEM::Data pzem1Data, pzem2Data, pzem3Data;

// Global Task pointers - initialized in setup()
Task* task1 = nullptr;
Task* mqttReconnectTimer = nullptr;

void setup() {
  // Disable watchdog completely during setup
  esp_task_wdt_deinit();
  
  delay(3000);
  
  Serial.begin(115200);
  delay(2000);
  Serial.println(F("\n=== POWERMETER STARTING ==="));
  Serial.flush();
  
  Serial.println(F("Step 1: Initializing global objects..."));
  Serial.flush();
  
  // Initialize objects dynamically to avoid stack overflow during global initialization
  mqttmediator = new MQTTMediator();
  tuplecorefactory = new TupleCoreFactory();
  commandcenter = new CommandCenter();
  mqttcommand = new MQTT_Command(mqttmediator, *commandcenter, ""); // Empty path - load later
  serialcommand = new Serial_Command(*commandcenter);
  webserver = new AsyncWebServer(80);
  taskManager = new Scheduler();
  wifimanager = new WifiManager();
  
  sh = new struct_hardwares(*webserver, *tuplecorefactory, *commandcenter, *mqttcommand, mqttmediator, *mqttmediator, *wifimanager);
  wifiTool = new WifiTool(*sh);
  
  pzem1 = new Mycila::PZEM();
  pzem2 = new Mycila::PZEM();
  pzem3 = new Mycila::PZEM();
  
  // Initialize tasks
  task1 = new Task(1000, TASK_FOREVER, &t1Cb_secTimer, taskManager, false);
  mqttReconnectTimer = new Task(10000, TASK_FOREVER, &connectToMqtt, taskManager, false);
  
  Serial.println(F("Step 1: Global objects DONE"));
  Serial.flush();
  
  // CRITICAL FIX: Initialize SPIFFS BEFORE loading settings that depend on it
  Serial.println(F("Step 2: Starting SPIFFS initialization..."));
  Serial.flush();
  
  if (!SPIFFS.begin(false)) {
    Serial.println(F("SPIFFS initialization failed! Trying format..."));
    if (!SPIFFS.begin(true)) {
      Serial.println(F("SPIFFS format failed! Using defaults"));
    }
  }
  Serial.println(F("Step 2: SPIFFS DONE"));
  Serial.flush();

  Serial.println(F("Step 3: Loading MQTT settings..."));
  Serial.flush();

  sh->mqttstruct.loadMQTTsettings();
    
  mqttmediator->setServer(sh->mqttstruct.mqttServer.c_str(),sh->mqttstruct.mqttPort);
  //mqttmediator.setServer(IPAddress ip, uint16_t port);
  mqttmediator->setKeepAlive(sh->mqttstruct.mqttKeepAlive);
  mqttmediator->setOnMediatorConnect(onMqttMediatorConnect,false);
  mqttmediator->setOnMediatorDisconnect(onMqttMediatorDisconnect,false);
  mqttmediator->setClientId(sh->mqttstruct.mqttClientId.c_str());
  mqttmediator->setCleanSession(sh->mqttstruct.mqttCleanSession);
  mqttmediator->setMaxTopicLength(64);
  mqttmediator->setCredentials(sh->mqttstruct.mqttUser.c_str(),sh->mqttstruct.mqttPassword.c_str());
  mqttmediator->setWill(sh->mqttstruct.mqttWillTopic.c_str(),sh->mqttstruct.mqttQoS,sh->mqttstruct.mqttRetain,sh->mqttstruct.mqttWillText.c_str());

  mqttcommand->loadMQTTCommandsettings();//command topic, response topic, qos
  mqttcommand->setCommandTopic((mqttcommand->getCommandTopic()=="")?"command":mqttcommand->getCommandTopic());//set default command topic if not set
  mqttcommand->setResponseTopic((mqttcommand->getResponseTopic()=="")?"response":mqttcommand->getResponseTopic());//set default response topic if not set
  if(mqttcommand->getQos()<0 || mqttcommand->getQos()>2)
  {
        mqttcommand->setQos(2);
  }
  mqttcommand->saveMQTTCommandsettings();
    
  Serial.println("MQTTCommand command topic: "+ mqttcommand->getCommandTopic() +" Response topic: "+ mqttcommand->getResponseTopic() +" Qos: "+ String(mqttcommand->getQos()));

  mqttcommand->setOnSubscribe([&](uint16_t packetid, uint8_t qos){Serial.println("MQTT_Command:Successful subscription.");});
  mqttcommand->setOnUnsubscribe([&](uint16_t packetid){Serial.println("MQTT_Command:Successful unsubscription.");});
  mqttcommand->setOnDisconnect([&](AsyncMqttClientDisconnectReason reason){Serial.println("MQTT_Command: Disconnected. Reason: "+ String(uint8_t(reason)));});


  //tuplecorefactory->addDeviceInitFunction(&wifimanager,"wifimanager");

  Serial.println(F("Step 3: DONE"));
  Serial.flush();
  
  Serial.println(F("Step 4: Version info..."));
  Serial.print(String(F("Version: ")) + getVersion());
  Serial.print(F(" Compiled: "));
  Serial.print(String(__DATE__) + String(F("-")));
  Serial.println(__TIME__);
  Serial.printf("CPU: %u MHz, Free Heap: %u bytes\n", ESP.getCpuFreqMHz(), ESP.getFreeHeap());
  Serial.flush();
  Serial.println(F("Step 4: DONE"));
  Serial.flush();

  // Initialize components that need SPIFFS
  Serial.println(F("Step 5: Starting BasicOTA..."));
  Serial.flush();
  BasicOTA.begin(webserver);
  Serial.println(F("Step 5: BasicOTA DONE"));
  Serial.flush();
  
  Serial.println(F("Step 6: Starting WiFiManager..."));
  Serial.flush();
  wifimanager->begin(); // Now safe - SPIFFS is ready and WifiManager has error handling
  Serial.println(F("Step 6: WiFiManager DONE"));
  Serial.flush();
  
  Serial.println(F("Step 7: Starting WiFiTool..."));
  Serial.flush();
  wifiTool->begin();
  Serial.println(F("Step 7: WiFiTool DONE"));
  Serial.flush();
  
  // MQTT event handlers
  Serial.println(F("Step 8: Setting up MQTT events..."));
  Serial.flush();
  #ifdef ARDUINO_ARCH_ESP32
  WiFi.onEvent(onWifiConnect,  ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(onWifiDisconnect, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  #elif defined(ARDUINO_ARCH_ESP8266)
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  #endif
  Serial.println(F("Step 8: MQTT events DONE"));
  Serial.flush();
  
Serial.println(F("Step 8.5: Setting up PZEM callbacks..."));
Serial.flush();

pzem1->setCallback([](const Mycila::PZEM::EventType eventType, const Mycila::PZEM::Data& data) {
  
  if(eventType == Mycila::PZEM::EventType::EVT_READ_ERROR) {
    Serial.println("L1 READ_ERROR");
    Serial.flush();
    return;
  }
  if(eventType == Mycila::PZEM::EventType::EVT_READ_TIMEOUT){
    Serial.println("L1 READ_TIMEOUT");
    Serial.flush();
    return;
  }
  if(eventType == Mycila::PZEM::EventType::EVT_READ){
    Serial.printf("L1 EVT_READ (SUCCESS): %f V, %f A, %f W\n", data.voltage, data.current, data.activePower);
    Serial.flush();
    return;
  }
  
  Serial.printf(" - %" PRIu32 "L1 EVT_UNKNOWN: %f V, %f A, %f W\n", millis(), data.voltage, data.current, data.activePower);
  Serial.flush();
    
  });
pzem2->setCallback([](const Mycila::PZEM::EventType eventType, const Mycila::PZEM::Data& data) {
    
    if(eventType == Mycila::PZEM::EventType::EVT_READ_ERROR) {
      Serial.println("L2 READ_ERROR");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ_TIMEOUT){
      Serial.println("L2 READ_TIMEOUT");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ){
      Serial.printf("L2 EVT_READ (SUCCESS): %f V, %f A, %f W\n", data.voltage, data.current, data.activePower);
      Serial.flush();
      return;
    }
    
    Serial.printf(" - %" PRIu32 "L2 EVT_UNKNOWN: %f V, %f A, %f W\n", millis(), data.voltage, data.current, data.activePower);
    Serial.flush();
    
  });
pzem3->setCallback([](const Mycila::PZEM::EventType eventType, const Mycila::PZEM::Data& data) {
    
    if(eventType == Mycila::PZEM::EventType::EVT_READ_ERROR) {
      Serial.println("L3 READ_ERROR");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ_TIMEOUT){
      Serial.println("L3 READ_TIMEOUT");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ){
      Serial.printf("L3 EVT_READ (SUCCESS): %f V, %f A, %f W\n", data.voltage, data.current, data.activePower);
      Serial.flush();
      return;
    }
  
    Serial.printf(" - %" PRIu32 "L3 EVT_UNKNOWN: %f V, %f A, %f W\n", millis(), data.voltage, data.current, data.activePower);
    Serial.flush();
  });
  
  pzem1->begin(Serial1, RX_pin, TX_pin, 0x01, true);
  pzem2->begin(Serial1, RX_pin, TX_pin, 0x02, true);
  pzem3->begin(Serial1, RX_pin, TX_pin, 0x03, true);
 
  Serial.println(F("Step 9: PZEM async initialization DONE"));
  Serial.flush();

  Serial.println(F("=== SETUP COMPLETE ==="));
  Serial.flush();
  // Re-enable watchdog with longer timeout
  Serial.println(F("Re-enabling watchdog..."));
  
  esp_task_wdt_init(30, true);
  esp_task_wdt_add(NULL);
  // Start tasks only after everything is initialized
  task1->enable();
  Serial.println(F("Tasks enabled"));
  Serial.flush();

  Serial.printf("Total heap: %d\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
}

void loop() {
  // Reset watchdog early in loop
  esp_task_wdt_reset();
  yield();
  

  wifiTool->process();

  yield();
  esp_task_wdt_reset(); // Extra watchdog feed
  

  taskManager->execute();
  yield();
  esp_task_wdt_reset(); // Extra watchdog feed

  commandcenter->process();
  
  yield();
  esp_task_wdt_reset(); // Extra watchdog feed
  
  serialcommand->processSerialInput();
  
  yield();
  esp_task_wdt_reset(); // Extra watchdog feed

  wifimanager->wifiAutoReconnect();
 
  yield();
  
}