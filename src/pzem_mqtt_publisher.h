#ifndef PZEM_MQTT_PUBLISHER_H
#define PZEM_MQTT_PUBLISHER_H


#include <Medclient.h>
#include <vector>
#include <MycilaPZEM.h>
#include "ArduinoJson.h"



class PzemMqttPublisher : public MedClient
{
private:
    std::vector<Mycila::PZEM*>& _pzems;
    u_int32_t                   _period; //if 0 then no publish
    u_int32_t                   _lastpublish=0;
    String                      _pathtojson;
public:
    PzemMqttPublisher(IMQTTMediator *mymqttmediator,std::vector<Mycila::PZEM*>& pzems,String pathtojson,u_int32_t period=0);
    ~PzemMqttPublisher(){};
    void Process();

    void sendOnce();
    void sendPeriodicaly(u_int32_t period);
    u_int32_t getPeriod(){return _period;};
    

    void onMessageCB(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    void onConnectCB(bool sessionpresent);
    // call from loop
};

#endif /* PZEM_MQTT_PUBLISHER_H */
