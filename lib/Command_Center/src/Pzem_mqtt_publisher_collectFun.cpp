#include "Pzem_mqtt_publisher_collectFun.h"

static const char device_type_relay[] PROGMEM = "PZEMMQTTPUBLISHER";

// template specialization for void CollectFuns<T>(T*)
template<> device_command_struct TupleCoreFactory::collectFuns<PzemMqttPublisher>(PzemMqttPublisher* publisher, String id_by_user)
{
    if(publisher==nullptr) return device_command_struct();
    
    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_type_relay);
    devcommst.id_by_user=id_by_user;

    //void PzemMqttPublisher::sendOnce()
    static const char pzempublisher_1_command[] PROGMEM = "sendOnce";
    std::shared_ptr<ft_V_V> fp( new ft_V_V(std::bind(&PzemMqttPublisher::sendOnce, publisher)));
    std::shared_ptr<IFunctionHolder> ifh (new FunctionHolder<ft_V_V>(fp));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(pzempublisher_1_command),ifh));

    //void PzemMqttPublisher::sendPeriodically(uint)
    static const char pzempublisher_2_command[] PROGMEM = "sendPeriodically";
    std::shared_ptr<ft_V_UI> fp2( new ft_V_UI(std::bind(&PzemMqttPublisher::sendPeriodically, publisher, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_V_UI>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(pzempublisher_2_command),ifh2));


    devcommst.device_commands.shrink_to_fit();
    return devcommst;


}
