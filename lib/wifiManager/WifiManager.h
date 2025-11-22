#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <vector>
#include <utility>
#include <DNSServer.h>
#include "SimpleJsonParser.h" //https://github.com/dzsoni/SimpleJsonParser

#ifdef ESP32
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

//#define  DEBUG_WIFIMANAGER 

#ifdef DEBUG_WIFIMANAGER
#define _WIFIMANAGER_PP(a) Serial.print(a);
#define _WIFIMANAGER_PL(a) Serial.println(a);
#else
#define _WIFIMANAGER_PP(a)
#define _WIFIMANAGER_PL(a)
#endif


#define WIFI_SECRETS_JSON      "/secrets.json"
#define DEFAULT_AP_PASSWORD    "password"
#define DEFAULT_AP_SSID        "Powermeter_AP"
#define WAIT_FOR_WIFI_TIME_OUT  6000UL
#define MAX_SSID_NUM            3
#define DEF_AP_IP               172, 217, 28, 1 //Set the accespoint ip
#define DEF_GATEWAY_IP          172, 217, 28, 1 //Set the default gateway ip
#define DEF_SUBNETMASK          255,255,255,0   //Set the subnet mask


class WifiManager
{
    private:
    unsigned long         _lastconnectatempt;
    bool                  _connecting;
    uint8_t               _lastconnectednetwork=0;
    String                _pathToSecretjson;
    String                _APpassword;
    String                _APssid;

    std::vector<std::pair<String,String>> _apscredentials;
    
   
    void                  setupSoftAP();

public:
    WifiManager();
    ~WifiManager();

    void begin();
    void disconnect();
    void addCredentials(String ssid, String pass, uint8_t index = 0);
    void setPathToSecretjson(String pathTosecretjson);
    void loadCredentials();
    void saveCredentials();
    void setAPpassword(String pass);//set the password for the SoftAP
    //void setAPssid(String ssid);//set the ssid for the SoftAP

    void  wifiAutoReconnect();// call this function in the loop continuously
};

#endif /* WIFIMANAGER_H */
