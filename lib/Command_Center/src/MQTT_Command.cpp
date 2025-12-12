#include "MQTT_Command.h"

void MQTT_Command::onMessageCB(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
     if(String(topic)==_commandtopic)
  {
  _MQTTCOMMAND_PL(String(__FUNCTION__)+": new command on "+ String(topic)+" topic.")
  
  String newcommand;
  newcommand.concat(payload,len);
  
  _mycommandcenter.addRawCommand(newcommand,this);
  }
}

void MQTT_Command::onConnectCB(bool sessionpresent)
{
   if(!sessionpresent && getCommandTopic()!="")
  {
    _MQTTCOMMAND_PL(String(__FUNCTION__)+": Subscribing to "+getCommandTopic())
    Serial.println("MQTT_Command is subscirbing to "+ _commandtopic);
    MedClient::subscribe(getCommandTopic(),getQos());//subcribe to the command topic
  }
}

MQTT_Command::MQTT_Command(IMQTTMediator *mymqttmediator, CommandCenter &mycommandcenter, String pathtojson):_mycommandcenter(mycommandcenter),_pathtojson(pathtojson)
{
    if(mymqttmediator==nullptr)return;
    
    MedClient::setMQTTMediator(mymqttmediator);

    MedClient::setOnMessage([&](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
    {onMessageCB(topic,payload,properties,len,index,total);});

    MedClient::setOnConnect([&](bool sessionpresent){onConnectCB(sessionpresent);});
}

MQTT_Command::MQTT_Command(IMQTTMediator *mymqttmediator, CommandCenter& mycommandcenter, String commandtopic, String responsetopic, uint8_t qosforpublish):_mycommandcenter(mycommandcenter)
{
    if(mymqttmediator==nullptr )return;
    MedClient::setMQTTMediator(mymqttmediator);
    _mycommandcenter = mycommandcenter;
    _responsetopic = responsetopic;
    _commandtopic = commandtopic;
    _qos = qosforpublish;

    MedClient::setOnMessage([&](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
    {onMessageCB(topic,payload,properties,len,index,total);});

    MedClient::setOnConnect([&](bool sessionpresent){onConnectCB(sessionpresent);});
}

void MQTT_Command::setResponseTopic(String responsetopic)
{
  if(responsetopic!="" && responsetopic!=_responsetopic )
  {
    _responsetopic = responsetopic;
    saveMQTTCommandsettings();
  }
}

void MQTT_Command::setCommandTopic(String commandtopic)
{
  if(commandtopic!="" && commandtopic!=_commandtopic )
  {
    MedClient::unsubscribe(_commandtopic.c_str());
    _commandtopic = commandtopic;
    saveMQTTCommandsettings();
    MedClient::subscribe(_commandtopic.c_str(),_qos);
  }
}

void MQTT_Command::saveMQTTCommandsettings()
{
    SimpleJsonWriter sjw;
    sjw.addKeyValue(F("COMMANDTOPIC"),_commandtopic);
    sjw.addKeyValue(F("RESPONSETOPIC"),_responsetopic);
    sjw.addKeyValue(F("MQTTCOMQOS"),String(_qos));
        
        File file = SPIFFS.open(MQTTCOMMAND_SETTINGS_JSON, "w");
        if (!file)
        {
            Serial.println(F("Error opening file for writing."));
            return;
        }
        file.print(sjw.getJsonString());
        file.flush();
        file.close();
}

void MQTT_Command::loadMQTTCommandsettings()
{
  SimpleJsonParser jsp;
  if(_pathtojson!="" && SPIFFS.exists(_pathtojson))
    {
     _commandtopic  = jsp.getValueByKeyFromFile(F(MQTTCOMMAND_SETTINGS_JSON), F("COMMANDTOPIC"));
     _responsetopic = jsp.getValueByKeyFromFile(F(MQTTCOMMAND_SETTINGS_JSON), F("RESPONSETOPIC"));
     _qos           = jsp.getValueByKeyFromFile(F(MQTTCOMMAND_SETTINGS_JSON), F("MQTTCOMQOS")).toInt();
    }
    else
    {
      String((String(__FUNCTION__))+": Json file  not found."+_pathtojson);
      return;
    }
}

void MQTT_Command::processResponse(String response)
{
  _MQTTCOMMAND_PL(String(__FUNCTION__)+"This is the response: "+response);
  if(_responsetopic=="")return;
  publish(_responsetopic.c_str(),_qos,0,response.c_str(),response.length(),0,0);
}
