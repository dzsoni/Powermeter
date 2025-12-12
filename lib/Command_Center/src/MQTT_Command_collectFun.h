#ifndef MQTT_COMMAND_COLLECTFUN_H
#define MQTT_COMMAND_COLLECTFUN_H

#ifndef NO_NEED_MQTTCOMMAND_COLLECTFUN

#include "wstring.h"
#include <memory>
#include "FunctionHolder.h"
#include "MQTT_Command.h"
#include "enum_CommandRepo.h"
#include "TupleCoreFactory.h"

template<> device_command_struct TupleCoreFactory::collectFuns<MQTT_Command>(MQTT_Command* mqttcomm, String id_by_user);

#endif
#endif /* MQTT_COMMAND_COLLECTFUN_H */
