#include "struct_hardwares.h"
#include "WString.h"




void mqtt_settings_struct::loadMQTTsettings()
{
  SimpleJsonParser sjp;
  mqtt_Tempsens_Vector.clear();
  std::vector<std::pair<String, String>> vec;
  vec = sjp.extractKeysandValuesFromFile(MQTT_SETTINGS_JSON);
  for (std::pair e : vec)
  {
    if (e.first == F("SERVERADDRESS"))
    {
      mqttServer = e.second.c_str();
      continue;
    }
    if (e.first == F("SERVERPORT"))
    {
      mqttPort = (uint16_t)e.second.toInt();
      continue;
    }
    if (e.first == F("USERNAME"))
    {
      mqttUser = e.second.c_str();
      continue;
    }
    if (e.first == F("PASSWORD"))
    {
      mqttPassword = e.second.c_str();
      continue;
    }
    if (e.first == F("CLIENTID"))
    {
      if(e.second != "")
      {
      mqttClientId = e.second.c_str();
      }
      else
      {
      mqttClientId = "ESP8266";
      }
      continue;
    }
    if (e.first == F("CLEANSESSION"))
    {
      mqttCleanSession = (e.second == "true") ? true : false;
      continue;
    }
    if (e.first == F("RETAIN"))
    {
      mqttRetain = (e.second == "true") ? true : false;
      continue;
    }
    if (e.first == F("KEEPALIVE"))
    {
      mqttKeepAlive = (uint16_t)e.second.toInt();
      continue;
    }
    if (e.first == F("WILLTOPIC"))
    {
      mqttWillTopic = e.second;
      continue;
    }
    if (e.first == F("WILLQOS"))
    {
      mqttQoS = (uint8_t)e.second.toInt();
      continue;
    }
     if (e.first == F("WILLTEXT"))
    {
      mqttWillText = e.second;
      continue;
    }
     if (e.first.indexOf(":topic") >0)
    {
      int index= e.first.substring(0,e.first.indexOf(":topic")).toInt();
      for(std::pair i : vec)
      {
        if(i.first.indexOf(":sens")>0)
        {
          int z= i.first.substring(0,i.first.indexOf(":sens")).toInt();
          if(z==index)
          {
            for(std::pair x : vec)
            {
              if(x.first.indexOf(":qos")>0)
              {
                int y=x.first.substring(0,x.first.indexOf(":qos")).toInt();
                if(y==index)
                {
                  mqtt_Tempsens_Vector.emplace_back((mqtt_tempsens_settings){e.second,i.second,x.second.toInt()});
                  goto leave;
                }
              }
            }
          }

        }
      }
      leave:
      continue;
    }
  }
}

void mqtt_settings_struct::saveMQTTsettings()
{
  SimpleJsonWriter sjw;
  
  
  sjw.addKeyValue(F("SERVERADDRESS"),mqttServer);
  sjw.addKeyValue(F("SERVERPORT"),String(mqttPort));
  sjw.addKeyValue(F("USERNAME"),mqttUser);
  sjw.addKeyValue(F("PASSWORD"),mqttPassword);
  sjw.addKeyValue(F("CLIENTID"),mqttClientId);
  sjw.addKeyValue(F("CLEANSESSION"),(mqttCleanSession)? "true" : "false");
  sjw.addKeyValue(F("KEEPALIVE"),String(mqttKeepAlive));
  sjw.addKeyValue(F("WILLRETAIN"),(mqttRetain)? "true" : "false");
  sjw.addKeyValue(F("WILLTOPIC"),mqttWillTopic);
  sjw.addKeyValue(F("WILLQOS"),String(mqttQoS));
  sjw.addKeyValue(F("WILLTEXT"),mqttWillText);

  for (unsigned int i = 0; i < mqtt_Tempsens_Vector.size(); i++)
  {
    sjw.addKeyValue(String(i) + ":topic", mqtt_Tempsens_Vector.at(i).topic);
    sjw.addKeyValue(String(i) + ":sens", mqtt_Tempsens_Vector.at(i).sensName);
    sjw.addKeyValue(String(i) + ":qos", String(mqtt_Tempsens_Vector.at(i).QoS));
  }
  File file = SPIFFS.open(MQTT_SETTINGS_JSON, "w");
  if (!file)
  {
    //(F("Error opening file for writing."));
    return;
  }
  file.print(sjw.getJsonString());
  file.flush();
  file.close();
}
