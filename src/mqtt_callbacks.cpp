#include "mqtt_callbacks.h"

#include <Arduino.h>
#include <AsyncMqttClient.h>
#include <SimpleJsonParser.h>
#include <TaskSchedulerDeclarations.h>
#include <struct_hardwares.h>
#include <vector>
#include <utility>
#include "CommandCenter.h"
#include "MQTTMediator.h"
#include "wifiTool.h"
#include "WifiManager.h"

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#endif

extern struct_hardwares sh;
//extern AsyncMqttClient mqttClient;
extern Task mqttReconnectTimer;
extern Task task2;//asyncMqtt
extern CommandCenter commandcenter;
extern MQTTMediator mqttmediator;
extern WifiManager wifimanager;



void connectToMqtt()
{
  if (WiFi.isConnected())
  {
    if (!mqttmediator.connected() && sh.mqttstruct.mqttServer !="" && sh.mqttstruct.mqttPort !=0)
    {
      Serial.println("Connecting to MQTT...");
      Serial.println("MQTTServer:"+sh.mqttstruct.mqttServer+" : "+sh.mqttstruct.mqttPort);
      mqttmediator.connect();
    }
    else if(!mqttmediator.connected() && (sh.mqttstruct.mqttServer =="" || sh.mqttstruct.mqttPort ==0))
    {
      Serial.println(F("Wrong MQTT server address or port."));
    }
  }
}
#ifdef ARDUINO_ARCH_ESP32
void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println(F("No Wi-Fi.Connecting to the MQTT server is paused."));
  mqttReconnectTimer.disable(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  task2.disable();//asyncTS write stop
}
#elif defined(ARDUINO_ARCH_ESP8266)
void onWifiDisconnect(const WiFiEventStationModeDisconnected &event)
{
  Serial.println(F("No Wi-Fi.Connecting to the MQTT server is paused."));
  mqttReconnectTimer.disable(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  task2.disable();//asyncTS write stop
}
#endif

#ifdef ARDUINO_ARCH_ESP32
void onWifiConnect(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Connected to Wi-Fi.");
  mqttReconnectTimer.enableDelayed();
  task2.enableDelayed(); //asyncTS write enable
}
#elif defined(ARDUINO_ARCH_ESP8266)
void onWifiConnect(const WiFiEventStationModeGotIP &event)
{
  Serial.println("Connected to Wi-Fi.");
  mqttReconnectTimer.enableDelayed();
  task2.enableDelayed(); //mqtt write
}
#endif

void onMqttMediatorConnect(bool sessionPresent)
{
  Serial.println("Mediator connected to MQTT server.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  sh.publish(sh.mqttstruct.mqttWillTopic.c_str(),2, true, "ESP8266 connected.");
  //if(!sessionPresent), then subscribe all topics here
  mqttReconnectTimer.disable();
}

void onMqttMediatorDisconnect(AsyncMqttClientDisconnectReason reason)
{
  Serial.println("Mediator disconnected from MQTT.");

  if (WiFi.isConnected())
  {
    mqttReconnectTimer.enableDelayed();
  }
}




/*
void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.print(topic);
  Serial.print("  qos: ");
  Serial.print(properties.qos);
  Serial.print("  dup: ");
  Serial.print(properties.dup);
  Serial.print("  retain: ");
  Serial.print(properties.retain);
  Serial.print("  len: ");
  Serial.print(len);
  Serial.print("  index: ");
  Serial.print(index);
  Serial.print("  total: ");
  Serial.print(total);
  Serial.println("");
}

void onMqttPublish(uint16_t packetId)
{
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}*/