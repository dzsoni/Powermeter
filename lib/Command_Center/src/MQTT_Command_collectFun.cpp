#include  "MQTT_Command_collectFun.h"

#ifndef  NO_NEED_MQTTCOMMAND_COLLECTFUN

static const char device_type_mqttcommand[] PROGMEM = "MQTTCOMMAND";
// template specialization for void CollectFuns<T>(T*)

template<> device_command_struct TupleCoreFactory::collectFuns<MQTT_Command>(MQTT_Command* mqttcomm, String id_by_user)
{
    if(mqttcomm==nullptr) return device_command_struct();
    
    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_type_mqttcommand);
    devcommst.id_by_user=id_by_user;

    //void MQTT_Command::setResponseTopic(String topic)
    static const char mqttcommand_command2[] PROGMEM = "setResponseTopic";
    std::shared_ptr<ft_V_S> fp2( new ft_V_S(std::bind(&MQTT_Command::setResponseTopic, mqttcomm,std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_V_S>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(mqttcommand_command2),ifh2));

    //String MQTT_Command::getResponseTopic()
    static const char mqttcommand_command3[] PROGMEM = "getResponseTopic";
    std::shared_ptr<ft_S_V> fp3( new ft_S_V(std::bind(&MQTT_Command::getResponseTopic, mqttcomm)));
    std::shared_ptr<IFunctionHolder> ifh3 (new FunctionHolder<ft_S_V>(fp3));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(mqttcommand_command3),ifh3));

    //void MQTT_Command::setCommandTopic(String topic)
    static const char mqttcommand_command4[] PROGMEM = "setCommandTopic";
    std::shared_ptr<ft_V_S> fp4( new ft_V_S(std::bind(&MQTT_Command::setCommandTopic, mqttcomm,std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh4 (new FunctionHolder<ft_V_S>(fp4));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(mqttcommand_command4),ifh4));

    //String MQTT_Command::getCommandTopic()
    static const char mqttcommand_command5[] PROGMEM = "getCommandTopic";
    std::shared_ptr<ft_S_V> fp5( new ft_S_V(std::bind(&MQTT_Command::getCommandTopic, mqttcomm)));
    std::shared_ptr<IFunctionHolder> ifh5 (new FunctionHolder<ft_S_V>(fp5));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(mqttcommand_command5),ifh5));

    //void MQTT_Command::setQos(uint8_t qos)
    static const char mqttcommand_command6[] PROGMEM = "setQos";
    std::shared_ptr<ft_V_UC> fp6( new ft_V_UC(std::bind(&MQTT_Command::setQos, mqttcomm,std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh6 (new FunctionHolder<ft_V_UC>(fp6));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(mqttcommand_command6),ifh6));

    //unsigned char MQTT_Command::getQos()
    static const char mqttcommand_command7[] PROGMEM = "getQos";
    std::shared_ptr<ft_UC_V> fp7( new ft_UC_V(std::bind(&MQTT_Command::getQos, mqttcomm)));
    std::shared_ptr<IFunctionHolder> ifh7 (new FunctionHolder<ft_UC_V>(fp7));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(mqttcommand_command7),ifh7));
    
    devcommst.device_commands.shrink_to_fit();
    return devcommst;
}
#endif