#ifndef PZEM_MQTT_PUBLISHER_COLLECTFUN_H
#define PZEM_MQTT_PUBLISHER_COLLECTFUN_H

#include "TupleCoreFactory.h"
#include "PzemMqttPublisher.h"
#include "wstring.h"
#include <memory>
#include "FunctionHolder.h"
#include "enum_CommandRepo.h"

template<> device_command_struct TupleCoreFactory::collectFuns<PzemMqttPublisher>(PzemMqttPublisher* publisher,String id_by_user);


#endif /* PZEM_MQTT_PUBLISHER_COLLECTFUN_H */
