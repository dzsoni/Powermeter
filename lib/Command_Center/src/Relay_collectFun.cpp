#include  "Relay_collectFun.h"
#include  "enum_CommandRepo.h"
#ifndef NO_NEED_RELAY_COLLECTFUN

static const char device_type_relay[] PROGMEM = "RELAY";

// template specialization for void CollectFuns<T>(T*)
template<> device_command_struct TupleCoreFactory::collectFuns<Relay>(Relay* relay, String id_by_user)
{	
    if(relay==nullptr) return device_command_struct();

    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_type_relay);
    devcommst.id_by_user=id_by_user;
    
    //void Relay::On()
    static const char relay_1_command[] PROGMEM = "On";
    std::shared_ptr<ft_V_V> fp( new ft_V_V(std::bind(&Relay::On, relay)));
    std::shared_ptr<IFunctionHolder> ifh (new FunctionHolder<ft_V_V>(fp));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_1_command),ifh));
    

    //void Relay::Off()
    static const char relay_2_command[] PROGMEM = "Off";
    std::shared_ptr<ft_V_V> fp2( new ft_V_V(std::bind(&Relay::Off, relay)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_V_V>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_2_command), ifh2));
    

    //void Relay::setRelayState(bool state)
    static const char relay_3_command[] PROGMEM = "setRelayState";
    std::shared_ptr<ft_V_B> fp3 (new ft_V_B(std::bind(&Relay::setRelayState, relay, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh3 (new FunctionHolder<ft_V_B>(fp3));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_3_command), ifh3));
    

    //unsigned char Relay::getRelayPin();
    static const char relay_4_command[] PROGMEM = "getRelayPin";
    std::shared_ptr<ft_UC_V> fp4 (new ft_UC_V(std::bind(&Relay::getRelayPin, relay)));
    std::shared_ptr<IFunctionHolder> ifh4(new FunctionHolder<ft_UC_V>(fp4));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_4_command), ifh4));

   
    //void Relay::init(unsigned char pin, bool initstate, bool on_state_level)
    static const char relay_5_command[] PROGMEM = "init";
    std::shared_ptr<ft_V_UC_B_B> fp5 (new ft_V_UC_B_B    (std::bind(
                                                          &Relay::init,
                                                          relay,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2, 
                                                          std::placeholders::_3)));
    std::shared_ptr<IFunctionHolder> ifh5 (new FunctionHolder<ft_V_UC_B_B>(fp5));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_5_command), ifh5));

    //bool Relay::getRelayState();
    static const char relay_6_command[] PROGMEM = "getRelayState";
    std::shared_ptr<ft_B_V> fp6 (new ft_B_V(std::bind(&Relay::getRelayState, relay)));
    std::shared_ptr<IFunctionHolder> ifh6(new FunctionHolder<ft_B_V>(fp6));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_6_command), ifh6));

    //bool Relay::getOnStateLevel();
    static const char relay_7_command[] PROGMEM = "getOnStateLevel";
    std::shared_ptr<ft_B_V> fp7 (new ft_B_V(std::bind(&Relay::getOnStateLevel, relay)));
    std::shared_ptr<IFunctionHolder> ifh7(new FunctionHolder<ft_B_V>(fp7));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_7_command), ifh7));

    //bool Relay::getInitstate();
    static const char relay_8_command[] PROGMEM = "getInitState";
    std::shared_ptr<ft_B_V> fp8 (new ft_B_V(std::bind(&Relay::getInitState, relay)));
    std::shared_ptr<IFunctionHolder> ifh8(new FunctionHolder<ft_B_V>(fp8));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relay_8_command), ifh8));
    
    devcommst.device_commands.shrink_to_fit();
    return devcommst;
}
#endif