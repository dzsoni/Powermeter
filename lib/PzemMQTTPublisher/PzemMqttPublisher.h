#ifndef PZEMMQTTPUBLISHER_H
#define PZEMMQTTPUBLISHER_H


#include <Medclient.h>
#include <vector>
#include <MycilaPZEM.h>
#include "ArduinoJson.h"

// Forward declaration to avoid circular dependency
struct struct_hardwares;

struct pzem_serial_settings
{
    String serialname;
    String name;
    Mycila::PZEM* pzem;
};
struct pzem_serial_settings_struct
{
    std::vector<pzem_serial_settings> settings;
    u_int32_t period = 5;
    void loadPzemSerialSettings(String pathtojson);
};

class PzemMqttPublisher : public MedClient
{
private:
    struct_hardwares*            _sh;
    pzem_serial_settings_struct& _settings;
    u_int32_t                   _lastpublish=0;
    String                      _pathtojson;
public:
    PzemMqttPublisher(IMQTTMediator *mymqttmediator,pzem_serial_settings_struct& settings,String pathtojson,struct_hardwares* sh);
    ~PzemMqttPublisher(){};
    void Process();

    void sendOnce();
    void sendPeriodically(u_int32_t period);
    u_int32_t getPeriod(){return _settings.period;};
    
    void onConnectCB(bool sessionpresent);
    // call from loop
};

#endif /* PZEMMQTTPUBLISHER_H */
