#include "PzemMqttPublisher.h"
#include "..\..\src\struct_hardwares.h"
#include "..\wifiTool\include\definitions.h"
#include <SPIFFS.h>
#include <SimpleJsonParser.h>
#include <map>


PzemMqttPublisher::PzemMqttPublisher(IMQTTMediator *mymqttmediator,pzem_serial_settings_struct& settings,String pathtojson,struct_hardwares* sh,u_int32_t period):
                                     _settings(settings),
                                     _pathtojson(pathtojson),
                                     _sh(sh),
                                     _period(period)
{
    if(mymqttmediator==nullptr)return;
    MedClient::setMQTTMediator(mymqttmediator);
    MedClient::setOnConnect([&](bool sessionpresent){onConnectCB(sessionpresent);});
};

void PzemMqttPublisher::Process()
{
    if (_period > 0)
    {
        if (millis() - _lastpublish > _period*1000)
        {
            if(!MedClient::connected())return;
            for (int i = 0; i < _sh->mqttstruct.mqtt_Pzem_Vector.size(); i++)
            {
                // Get the mqtt settings for this pzem
                mqtt_pzem_settings& mqttSettings = _sh->mqttstruct.mqtt_Pzem_Vector[i];
                
                // Find the matching pzem by name
                Mycila::PZEM* foundPzem = nullptr;
                for (int j = 0; j < _settings.settings.size(); j++)
                {
                    if (_settings.settings[j].serialname == mqttSettings.pzemName)
                    {
                        foundPzem = _settings.settings[j].pzem;
                        break;
                    }
                }
                
                // If pzem found, publish its data
                if (foundPzem != nullptr)
                {
                    JsonDocument jdoc;
                    JsonObject root = jdoc.to<JsonObject>();
                    foundPzem->toJson(root);
                    
                    String json;
                    serializeJson(root, json);
                    MedClient::publish(mqttSettings.topic.c_str(), mqttSettings.QoS, false, json.c_str(), json.length());
                }
                yield();
            }
            _lastpublish = millis();
        }
    }
}

void PzemMqttPublisher::sendOnce()
{
    sendPeriodically(0);//stop sending periodicaly
    if(!MedClient::connected())return;
    for (int i = 0; i < _settings.settings.size(); i++)
    {
        JsonDocument jdoc;
        JsonObject root = jdoc.to<JsonObject>();
        _settings.settings[i].pzem->toJson(root);
        String json;
        serializeJson(root, json);
        MedClient::publish(String("Power/pzem/L"+String(i+1)).c_str(),0,false,json.c_str(),json.length());
    }
    
}

void PzemMqttPublisher::sendPeriodically(u_int32_t period)
{
    _period=period;
}

void PzemMqttPublisher::onConnectCB(bool sessionpresent)
{
    Serial.println("pzem mqtt publisher connected.");
}

void pzem_serial_settings_struct::loadPzemSerialSettings()
{
    // Check if SPIFFS is mounted and file exists before trying to load
    if (!SPIFFS.exists(PZEM_ADDRESS_JSON)) {
        Serial.println(F("PZEM address file not found, using defaults"));
        settings.clear();
        return;
    }
    
    SimpleJsonParser sjp;
    settings.clear();
    std::vector<std::pair<String, String>> vec;
    vec = sjp.extractKeysandValuesFromFile(PZEM_ADDRESS_JSON);
    
    // Create temporary storage for partial entries
    std::map<int, pzem_serial_settings> tempMap;
    
    for (std::pair<String, String> e : vec)
    {
        // Parse key format: "index:field"
        int colonPos = e.first.indexOf(":");
        if (colonPos > 0)
        {
            int index = e.first.substring(0, colonPos).toInt();
            String field = e.first.substring(colonPos + 1);
            
            // Get or create entry for this index
            if (tempMap.find(index) == tempMap.end())
            {
                tempMap[index] = pzem_serial_settings();
                tempMap[index].pzem = nullptr; // Initialize to nullptr
            }
            
            // Set appropriate field
            if (field == "ser")
            {
                tempMap[index].serialname = e.second;
            }
            else if (field == "name")
            {
                tempMap[index].name = e.second;
            }
        }
    }
    
    // Transfer from map to vector, maintaining sorted order by index
    for (auto& pair : tempMap)
    {
        settings.push_back(pair.second);
    }
    
    Serial.printf("Loaded %d PZEM serial settings\n", settings.size());
}
