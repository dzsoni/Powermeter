#ifndef STRUCT_HARDWARES_H
#define STRUCT_HARDWARES_H


#include <vector>
#include "Medclient.h"
#include "MQTTMediator.h"
#include <utility>
#include <enum_CommandRepo.h>
#include "..\wifiTool\include\definitions.h"
#include "CommandCenter.h"
#include "TupleCoreFactory.h"
#include "SimpleJsonWriter.h"
#include "MQTT_Command.h"
#include "ESPAsyncWebServer.h"


//Forward declared classes
class ControlLogic;

struct mqtt_tempsens_settings
{
    String topic;
    String sensName;
    unsigned int QoS;   
};
struct mqtt_settings_struct
{
     // MQTT variables
    String mqttUser;
    String mqttPassword;
    String mqttServer;
    uint   mqttPort=1883;
    uint   mqttKeepAlive=15;
    String mqttWillTopic;
    String mqttWillText;
    String mqttClientId;
    uint8_t mqttQoS=0;
    bool    mqttRetain=0;
    bool    mqttCleanSession=1;
    std::vector<struct mqtt_tempsens_settings> mqtt_Tempsens_Vector;
    
    void loadMQTTsettings();
    void saveMQTTsettings();
};



struct struct_hardwares:public MedClient
{   
    public:
    struct_hardwares(AsyncWebServer& webserver,
                     TupleCoreFactory& tuplefactory,
                     CommandCenter& comcenter,
                     MQTT_Command& mqttCommand,
                     IMQTTMediator* mqttMediator_for_sh,
                     MQTTMediator& mqttmediator,
                     WifiManager& wifimanager):
                    webserver(webserver),
                    tuplefactory(tuplefactory),
                    comcenter(comcenter),
                    mqttCommand(mqttCommand),
                    mqttmediator(mqttmediator),
                    wifimanager(wifimanager)
                    {MedClient::setMQTTMediator(mqttMediator_for_sh);};    
    
    AsyncWebServer& webserver;   
    TupleCoreFactory& tuplefactory;
    CommandCenter& comcenter;
    MQTT_Command& mqttCommand;  
    MQTTMediator& mqttmediator;
    WifiManager& wifimanager;
    mqtt_settings_struct mqttstruct;
};

#endif /* STRUCT_HARDWARES_H */
