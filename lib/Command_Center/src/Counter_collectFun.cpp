#include "Counter_collectFun.h"
#include  "enum_CommandRepo.h"
#ifndef NO_NEED_COUNTER_COLLECTFUN

static const char device_type_counter[] PROGMEM = "COUNTER";

// template specialization for void CollectFuns<T>(T*)
template<> device_command_struct TupleCoreFactory::collectFuns<Counter>(Counter* counter,String id_by_user)
{
    if(counter ==nullptr) return device_command_struct();
    
    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_type_counter);
    devcommst.id_by_user=id_by_user;
    
    //int Counter::getCounterValue()
    static const char counter_1_command[] PROGMEM = "getCounterValue";
    std::shared_ptr<ft_UI_V> fp1(new ft_UI_V(std::bind(&Counter::getCounterValue, counter)));
    std::shared_ptr<IFunctionHolder> ifh1 (new FunctionHolder<ft_UI_V>(fp1));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_1_command), ifh1));
    devcommst.device_commands.shrink_to_fit();

    //void Counter::setCounterValue(uint32_t newvalue)
    static const char counter_2_command[] PROGMEM = "setCounterValue";
    std::shared_ptr<ft_V_UI> fp2(new ft_V_UI(std::bind(&Counter::setCounterValue, counter, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_V_UI>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_2_command), ifh2));

    //void Counter::setPin(uint8_t newpin)
    static const char counter_3_command[] PROGMEM = "setPin";
    std::shared_ptr<ft_V_UC> fp3(new ft_V_UC(std::bind(&Counter::setPin, counter, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh3 (new FunctionHolder<ft_V_UC>(fp3));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_3_command), ifh3));

    //uint8_t Counter::getPin()
    static const char counter_4_command[] PROGMEM = "getPin";
    std::shared_ptr<ft_UC_V> fp4(new ft_UC_V(std::bind(&Counter::getPin, counter)));
    std::shared_ptr<IFunctionHolder> ifh4 (new FunctionHolder<ft_UC_V>(fp4));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_4_command), ifh4));

    //void Counter::setStableTime(uint32_t newtime)
    static const char counter_5_command[] PROGMEM = "setStableTime";
    std::shared_ptr<ft_V_UI> fp5(new ft_V_UI(std::bind(&Counter::setStableTime, counter, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh5 (new FunctionHolder<ft_V_UI>(fp5));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_5_command), ifh5));

    //uint32_t Counter::getStableTime()
    static const char counter_6_command[] PROGMEM = "getStableTime";
    std::shared_ptr<ft_UI_V> fp6(new ft_UI_V(std::bind(&Counter::getStableTime, counter)));
    std::shared_ptr<IFunctionHolder> ifh6 (new FunctionHolder<ft_UI_V>(fp6));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_6_command), ifh6));

    //void Counter::enableCounter()
    static const char counter_7_command[] PROGMEM = "enableCounter";
    std::shared_ptr<ft_V_V> fp7(new ft_V_V(std::bind(&Counter::enableCounter, counter)));
    std::shared_ptr<IFunctionHolder> ifh7 (new FunctionHolder<ft_V_V>(fp7));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_7_command), ifh7));

    //void Counter::disableCounter()
    static const char counter_8_command[] PROGMEM = "disableCounter";
    std::shared_ptr<ft_V_V> fp8(new ft_V_V(std::bind(&Counter::disableCounter, counter)));
    std::shared_ptr<IFunctionHolder> ifh8 (new FunctionHolder<ft_V_V>(fp8));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_8_command), ifh8));

    //bool Counter::isEnabled()
    static const char counter_9_command[] PROGMEM = "isCounterEnabled";
    std::shared_ptr<ft_B_V> fp9(new ft_B_V(std::bind(&Counter::isEnabled, counter)));
    std::shared_ptr<IFunctionHolder> ifh9 (new FunctionHolder<ft_B_V>(fp9));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_9_command), ifh9));

   //void Counter::setMqttPublishPeriod(uint32_t period)
   static const char counter_10_command[] PROGMEM = "setMqttPublishPeriod";
   std::shared_ptr<ft_V_UI> fp10(new ft_V_UI(std::bind(&Counter::setMqttPublishPeriod, counter, std::placeholders::_1)));
   std::shared_ptr<IFunctionHolder> ifh10 (new FunctionHolder<ft_V_UI>(fp10));
   devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_10_command), ifh10));

 
   //uint32_t Counter::getMqttPublishPeriod()
   static const char counter_11_command[] PROGMEM = "getMqttPublishPeriod";
   std::shared_ptr<ft_UI_V> fp11(new ft_UI_V(std::bind(&Counter::getMqttPublishPeriod, counter)));
   std::shared_ptr<IFunctionHolder> ifh11 (new FunctionHolder<ft_UI_V>(fp11));
   devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_11_command), ifh11));

   //void Counter::SetCounterTopic(String topic)
   static const char counter_12_command[] PROGMEM = "setCounterTopic";
   std::shared_ptr<ft_V_S> fp12(new ft_V_S(std::bind(&Counter::setCounterTopic, counter, std::placeholders::_1)));
   std::shared_ptr<IFunctionHolder> ifh12 (new FunctionHolder<ft_V_S>(fp12));
   devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_12_command), ifh12));

   //String Counter::getCounterTopic()
   static const char counter_13_command[] PROGMEM = "getCounterTopic";
   std::shared_ptr<ft_S_V> fp13(new ft_S_V(std::bind(&Counter::getCounterTopic, counter)));
   std::shared_ptr<IFunctionHolder> ifh13 (new FunctionHolder<ft_S_V>(fp13));
   devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_13_command), ifh13));
   
   //void Counter::saveMqttsettings()

   static const char counter_14_command[] PROGMEM = "saveMqttsettings";
   std::shared_ptr<ft_V_V> fp14(new ft_V_V(std::bind(&Counter::saveMqttsettings, counter)));
   std::shared_ptr<IFunctionHolder> ifh14 (new FunctionHolder<ft_V_V>(fp14));
   devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_14_command), ifh14));

   //void Counter::loadMqttsettings()
   static const char counter_15_command[] PROGMEM = "loadMqttsettings";
   std::shared_ptr<ft_V_V> fp15(new ft_V_V(std::bind(&Counter::loadMqttsettings, counter)));
   std::shared_ptr<IFunctionHolder> ifh15 (new FunctionHolder<ft_V_V>(fp15));
   devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(counter_15_command), ifh15));
    
    devcommst.device_commands.shrink_to_fit();
    return devcommst;
}






#endif