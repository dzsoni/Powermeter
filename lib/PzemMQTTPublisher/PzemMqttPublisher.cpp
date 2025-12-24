#include "PzemMqttPublisher.h"
#include "..\..\src\struct_hardwares.h"
#include "..\wifiTool\include\definitions.h"
#include <SPIFFS.h>
#include <SimpleJsonParser.h>


PzemMqttPublisher::PzemMqttPublisher(IMQTTMediator *mymqttmediator,pzem_serial_settings_struct& settings,String pathtojson,struct_hardwares* sh):
                                     _settings(settings),
                                     _pathtojson(pathtojson),
                                     _sh(sh)
                                     
{
    if(mymqttmediator==nullptr)return;
    _settings.loadPzemSerialSettings(pathtojson);
    MedClient::setMQTTMediator(mymqttmediator);
    MedClient::setOnConnect([&](bool sessionpresent){onConnectCB(sessionpresent);});
};

void PzemMqttPublisher::Process()
{
    if (_settings.period > 0)
    {
        if (millis() - _lastpublish > _settings.period*1000)
        {
            if(!MedClient::connected())
            {
                //Serial.println("PzemMqttPublisher: Not connected to MQTT");
                return;
            }
            
            //Serial.printf("PzemMqttPublisher: Processing %d mqtt topics\n", _sh->mqttstruct.mqtt_Pzem_Vector.size());
            
            for (int i = 0; i < _sh->mqttstruct.mqtt_Pzem_Vector.size(); i++)
            {
                // Get the mqtt settings for this pzem
                mqtt_pzem_settings& mqttSettings = _sh->mqttstruct.mqtt_Pzem_Vector[i];
                //Serial.printf("Looking for PZEM with name: '%s'\n", mqttSettings.pzemName.c_str());
                
                // Find the matching pzem by name
                Mycila::PZEM* foundPzem = nullptr;
                for (int j = 0; j < _settings.settings.size(); j++)
                {
                    //Serial.printf("  Checking settings[%d]: serialname='%s', name='%s'\n", j, _settings.settings[j].serialname.c_str(), _settings.settings[j].name.c_str());
                    
                    if (_settings.settings[j].name == mqttSettings.pzemName)
                    {
                        //Serial.printf("Found matching PZEM: '%s'\n", _settings.settings[j].name.c_str());
                        foundPzem = _settings.settings[j].pzem;
                        break;
                    }
                }
                
                // If pzem found, publish its data
                if (foundPzem != nullptr)
                {
                    if (foundPzem->isEnabled())
                    {
                        JsonDocument jdoc;
                        JsonObject root = jdoc.to<JsonObject>();
                        foundPzem->toJson(root);
                        
                        String json;
                        serializeJson(root, json);
                        Serial.printf("Publishing to '%s': %s\n", mqttSettings.topic.c_str(), json.c_str());
                        MedClient::publish(mqttSettings.topic.c_str(), mqttSettings.QoS, false, json.c_str(), json.length());
                    }
                    else
                    {
                        Serial.printf("PZEM '%s' is not enabled\n", mqttSettings.pzemName.c_str());
                    }
                }
                else
                {
                    Serial.printf("No matching PZEM found for '%s'\n", mqttSettings.pzemName.c_str());
                }
                yield();
            }
            _lastpublish = millis();
        }
    }
}

void PzemMqttPublisher::sendOnce()
{
    sendPeriodically(_settings.period);
    if(!MedClient::connected())return;
    
    Serial.printf("PzemMqttPublisher: Sending once, Processing %d mqtt topics\n", _sh->mqttstruct.mqtt_Pzem_Vector.size());
    
    for (int i = 0; i < _sh->mqttstruct.mqtt_Pzem_Vector.size(); i++)
    {
        // Get the mqtt settings for this pzem
        mqtt_pzem_settings& mqttSettings = _sh->mqttstruct.mqtt_Pzem_Vector[i];
        Serial.printf("Looking for PZEM with name: '%s'\n", mqttSettings.pzemName.c_str());
        
        // Find the matching pzem by name
        Mycila::PZEM* foundPzem = nullptr;
        for (int j = 0; j < _settings.settings.size(); j++)
        {
            if (_settings.settings[j].name == mqttSettings.pzemName)
            {
                Serial.printf("Found matching PZEM: '%s'\n", _settings.settings[j].name.c_str());
                foundPzem = _settings.settings[j].pzem;
                break;
            }
        }
        
        // If pzem found, publish its data
        if (foundPzem != nullptr)
        {
            if (foundPzem->isEnabled())
            {
                JsonDocument jdoc;
                JsonObject root = jdoc.to<JsonObject>();
                foundPzem->toJson(root);
                
                String json;
                serializeJson(root, json);
                Serial.printf("Publishing to '%s': %s\n", mqttSettings.topic.c_str(), json.c_str());
                MedClient::publish(mqttSettings.topic.c_str(), mqttSettings.QoS, false, json.c_str(), json.length());
            }
            else
            {
                Serial.printf("PZEM '%s' is not enabled\n", mqttSettings.pzemName.c_str());
            }
        }
        else
        {
            Serial.printf("No matching PZEM found for '%s'\n", mqttSettings.pzemName.c_str());
        }
        yield();
    }
    
}

void PzemMqttPublisher::sendPeriodically(u_int32_t period)
{
    _settings.period=period;
}

void PzemMqttPublisher::onConnectCB(bool sessionpresent)
{
    Serial.println("pzem mqtt publisher connected.");
}

void pzem_serial_settings_struct::loadPzemSerialSettings(String pathtojson)
{
    // Check if SPIFFS is mounted and file exists before trying to load
    if (!SPIFFS.exists(pathtojson)) {
        Serial.println(F("PZEM address file not found, using existing settings"));
        return;
    }
    
    SimpleJsonParser sjp;
    std::vector<std::pair<String, String>> vec;
    vec = sjp.extractKeysandValuesFromFile(pathtojson);
    
    //Serial.printf("Loaded %d key-value pairs from JSON\n", vec.size());
    
    // Update existing settings entries by index
    // Format: "0:name" -> "L1", "1:name" -> "L2", etc.
    for (const std::pair<String, String>& e : vec)
    {
        // Find the colon position to split index and field
        int colonPos = e.first.indexOf(":");
        if (colonPos > 0)
        {
            int index = e.first.substring(0, colonPos).toInt();
            String field = e.first.substring(colonPos + 1);
            
            //Serial.printf("Processing: key='%s', index=%d, field='%s', value='%s'\n", e.first.c_str(), index, field.c_str(), e.second.c_str());
            
            // Only update if index is within bounds
            if (index >= 0 && index < settings.size())
            {
                if (field == "name")
                {
                    settings[index].name = e.second;
                    //Serial.printf("Set settings[%d].name =  '%s'\n", index, e.second.c_str());
                }
                else if (field == "ser")
                {
                    // Verify or update serialname
                    if (settings[index].serialname == "" || settings[index].serialname == e.second)
                    {
                        settings[index].serialname = e.second;
                        //Serial.printf("Set settings[%d].serialname = '%s'\n", index, e.second.c_str());
                    }
                }
                
            }
        }
    }
    /*
    // Display final settings
    Serial.println("Final PZEM settings after load:");
    for (size_t i = 0; i < settings.size(); i++)
    {
        Serial.printf("  [%d] serialname='%s', name='%s', pzem=%s\n",
                    i, settings[i].serialname.c_str(), settings[i].name.c_str(),
                    (settings[i].pzem != nullptr) ? "OK" : "NULL");
    }
    */
    // Load period from MQTT settings JSON
    if (SPIFFS.exists(MQTT_SETTINGS_JSON)) {
        std::vector<std::pair<String, String>> mqttVec;
        mqttVec = sjp.extractKeysandValuesFromFile(MQTT_SETTINGS_JSON);
        
        for (const std::pair<String, String>& e : mqttVec)
        {
            if (e.first == "PERIOD" || e.first == "period")
            {
                period = (u_int32_t)strtoul(e.second.c_str(), NULL, 10);
                Serial.printf("Loaded period = %u\n", period);
                break;
            }
        }
    }
}
