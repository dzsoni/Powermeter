#ifndef MQTT_CALLBACKS_H
#define MQTT_CALLBACKS_H


#include  <AsyncMqttClient.h>

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266) 
#include <ESP8266WiFi.h>
#endif

void connectToMqtt();

#ifdef ARDUINO_ARCH_ESP32
void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ARDUINO_ARCH_ESP8266) 
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
#endif

#ifdef ARDUINO_ARCH_ESP32
void onWifiConnect(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ARDUINO_ARCH_ESP8266)
void onWifiConnect(const WiFiEventStationModeGotIP& event);
#endif

void onMqttMediatorConnect(bool sessionPresent);
void onMqttMediatorDisconnect(AsyncMqttClientDisconnectReason reason);

#endif /* MQTT_CALLBACKS_H */
