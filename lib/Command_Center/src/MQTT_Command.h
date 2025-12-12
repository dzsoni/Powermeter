#ifndef MQTT_COMMAND_H
#define MQTT_COMMAND_H

#include "MQTTMediator.h"
#include "Medclient.h"
#include "WString.h"
#include "CommandCenter.h"
#include "IResponseTaker.h"
#include "SimpleJsonParser.h"
#include "SimpleJsonWriter.h"


//#define DEBUG_MQTT_COMMAND //comment out or not 
#ifdef DEBUG_MQTT_COMMAND
#define _MQTTCOMMAND_PP(a) Serial.print(a);
#define _MQTTCOMMAND_PL(a) Serial.println(a);
#else
#define _MQTTCOMMAND_PP(a)
#define _MQTTCOMMAND_PL(a)
#endif

#ifndef MQTT_SETTINGS_JSON
#define MQTT_SETTINGS_JSON          "/mqtt.json"
#endif
#ifndef MQTTCOMMAND_SETTINGS_JSON
#define MQTTCOMMAND_SETTINGS_JSON   "/mqttcommand.json"
#endif
#ifndef MQTTCOMMANDDEVICEIDS_JSON
#define MQTTCOMMANDDEVICEIDS_JSON   "/mqtt_device_ids.json"
#endif

class MQTT_Command : public MedClient, public IResponseTaker
{
private:
    CommandCenter &_mycommandcenter;
    String _pathtojson = "";
    String _responsetopic = "";
    String _commandtopic = "";
    uint8_t _qos = 0; // qos for publish/subscribe commandtopic/responsetopic
    void onMessageCB(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    void onConnectCB(bool sessionpresent);

public:
    MQTT_Command(IMQTTMediator *mymqttmediator, CommandCenter &mycommandcenter,String pathtojson);
    MQTT_Command(IMQTTMediator *mymqttmediator, CommandCenter &mycommandcenter,String commandtopic, String responsetopic, uint8_t qosforpublish);
    ~MQTT_Command(){};
    void    setQos(unsigned char qos) { _qos = qos; saveMQTTCommandsettings(); }; // qos  for publish/subscribe
    unsigned char getQos() { return _qos; };           // qos for publish/subscribe
    void    setResponseTopic(String responsetopic);
    String  getResponseTopic() { return _responsetopic; };
    void    setCommandTopic(String commandtopic);
    String  getCommandTopic() { return _commandtopic; };
    void    setPathToJson(String pathtojson) { if(pathtojson!="")_pathtojson = pathtojson;};
    void    saveMQTTCommandsettings();
    void    loadMQTTCommandsettings();

    void processResponse(String response) override;
};
#endif /* MQTT_COMMAND_H */
