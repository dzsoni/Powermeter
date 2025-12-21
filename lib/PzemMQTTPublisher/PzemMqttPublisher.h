#ifndef PZEM_MQTT_PUBLISHER_H
#define PZEM_MQTT_PUBLISHER_H


#include <Medclient.h>
#include <vector>
#include <MycilaPZEM.h>
#include "ArduinoJson.h"

struct pzem_serial_settings
{
    String serialname;
    Mycila::PZEM* pzem;
};

class PzemMqttPublisher : public MedClient
{
private:
    std::vector<pzem_serial_settings>& _pzems;
    u_int32_t                   _period; //if 0 then no publish
    u_int32_t                   _lastpublish=0;
    String                      _pathtojson;
public:
    PzemMqttPublisher(IMQTTMediator *mymqttmediator,std::vector<pzem_serial_settings>& pzems,String pathtojson,u_int32_t period=0);
    ~PzemMqttPublisher(){};
    void Process();

    void sendOnce();
    void sendPeriodically(u_int32_t period);
    u_int32_t getPeriod(){return _period;};
    
    void onConnectCB(bool sessionpresent);
    // call from loop
};

#endif /* PZEM_MQTT_PUBLISHER_H */
