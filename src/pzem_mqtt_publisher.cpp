#include "pzem_mqtt_publisher.h"


PzemMqttPublisher::PzemMqttPublisher(IMQTTMediator *mymqttmediator,std::vector<Mycila::PZEM*>& pzems,String pathtojson,u_int32_t period):
                                     _pzems(pzems),
                                     _pathtojson(pathtojson),
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
            for (int i = 0; i < _pzems.size(); i++)
            {
                JsonDocument jdoc;
                JsonObject root = jdoc.to<JsonObject>();
                _pzems[i]->toJson(root);
                String json;
                serializeJson(root, json);
                MedClient::publish(String("L" + String(i)).c_str(), 0, false, json.c_str(), json.length());
                yield();
            }
            _lastpublish = millis();
        }
    }
}

void PzemMqttPublisher::sendOnce()
{
    sendPeriodicaly(0);//stop sending periodicaly
    if(!MedClient::connected())return;
    for (int i = 0; i < _pzems.size(); i++)
    {
        JsonDocument jdoc;
        JsonObject root = jdoc.to<JsonObject>();      
        _pzems[i]->toJson(root);
        String json;
        serializeJson(root, json);
        MedClient::publish(String("L"+String(i)).c_str(),0,false,json.c_str(),json.length());
    }
    
}

void PzemMqttPublisher::sendPeriodicaly(u_int32_t period)
{
    _period=period;
}

void PzemMqttPublisher::onConnectCB(bool sessionpresent)
{
    Serial.println("pzem mqtt publisher connected.");
}