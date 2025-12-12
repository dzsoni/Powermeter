#include  "CommandCenter_collectFun.h"
#ifndef  NO_NEED_COMMANDCENTER_COLLECTFUN

static const char device_commandcenter[] PROGMEM = "COMMANDCENTER";

// template specialization for void CollectFuns<T>(T*)

template<> device_command_struct TupleCoreFactory::collectFuns<CommandCenter>(CommandCenter* commcent, String id_by_user)
{	
    if(commcent==nullptr) return device_command_struct();
    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_commandcenter);
    devcommst.id_by_user=id_by_user;

    //void CommandCenter::numberOfDevs()
    static const char commandcenter_command1[] PROGMEM = "numberOfDevs";
    std::shared_ptr<ft_UI_V> fp( new ft_UI_V(std::bind(&CommandCenter::numberOfDevs, commcent)));
    std::shared_ptr<IFunctionHolder> ifh (new FunctionHolder<ft_UI_V>(fp));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(commandcenter_command1),ifh));

    //String CommandCenter::getDeviceIdByIndex(int)
    static const char commandcenter_command2[] PROGMEM = "getDeviceTypeByIndex";
    std::shared_ptr<ft_S_UC> fp1( new ft_S_UC(std::bind(&CommandCenter::getDeviceTypeByIndex, commcent,std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh1 (new FunctionHolder<ft_S_UC>(fp1));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(commandcenter_command2),ifh1));

    //String CommandCenter::getNthCommandInNthDevice(uint,uint)
    static const char commandcenter_command3[] PROGMEM = "getNthCommandOfNthDevice";
    std::shared_ptr<ft_S_UI_UI> fp2( new ft_S_UI_UI(std::bind(&CommandCenter::getNthCommandOfNthDevice, commcent,std::placeholders::_1,std::placeholders::_2)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_S_UI_UI>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(commandcenter_command3),ifh2));

   //void CommandCenter::echoMessageIdOn()
    static const char commandcenter_command4[] PROGMEM = "echoMessageIdOn";
    std::shared_ptr<ft_V_V> fp3( new ft_V_V(std::bind(&CommandCenter::echoMessageIdOn, commcent)));
    std::shared_ptr<IFunctionHolder> ifh3 (new FunctionHolder<ft_V_V>(fp3));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(commandcenter_command4),ifh3));

    //void CommandCenter::echoMessageIdOff()    
    static const char commandcenter_command5[] PROGMEM = "echoMessageIdOff";
    std::shared_ptr<ft_V_V> fp4( new ft_V_V(std::bind(&CommandCenter::echoMessageIdOff, commcent)));
    std::shared_ptr<IFunctionHolder> ifh4 (new FunctionHolder<ft_V_V>(fp4));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(commandcenter_command5),ifh4));

    //String CommandCenter::getDeviceIdByIndex(uint)
    static const char commandcenter_command6[] PROGMEM = "getDeviceIdByIndex";
    std::shared_ptr<ft_S_UC> fp5( new ft_S_UC(std::bind(&CommandCenter::getDeviceIdByIndex, commcent,std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh5 (new FunctionHolder<ft_S_UC>(fp5));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(commandcenter_command6),ifh5));

    devcommst.device_commands.shrink_to_fit();

    return devcommst;
}
#endif