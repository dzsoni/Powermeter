#include "ControlLogicManager_collectFun.h"

#ifndef  NO_NEED_CONTROLLOGICMANAGER_COLLECTFUN

static const char device_controllogicmanager[] PROGMEM = "LOGICMANAGER";

template<> device_command_struct TupleCoreFactory::collectFuns<ControlLogicManager>(ControlLogicManager* CLM, String id_by_user)
{	
    if(CLM==nullptr) return device_command_struct();
    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_controllogicmanager);
    devcommst.id_by_user=id_by_user;
 
    //bool ControlLogicManager::IsEnabled()
    static const char controllogicmanager_1_command[] PROGMEM = "IsEnabled";
    std::shared_ptr<ft_B_V> fp1( new ft_B_V(std::bind(&ControlLogicManager::IsEnabled, CLM)));
    std::shared_ptr<IFunctionHolder> ifh1 (new FunctionHolder<ft_B_V>(fp1));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(controllogicmanager_1_command),ifh1));

    //void ControlLogicManager::Enable()
    static const char controllogicmanager_2_command[] PROGMEM = "Enable";
    std::shared_ptr<ft_V_V> fp2( new ft_V_V(std::bind(&ControlLogicManager::Enable, CLM)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_V_V>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(controllogicmanager_2_command),ifh2));

    //void ControlLogicManager::Disable()    
    static const char controllogicmanager_3_command[] PROGMEM = "Disable";    
    std::shared_ptr<ft_V_V> fp3( new ft_V_V(std::bind(&ControlLogicManager::Disable, CLM)));
    std::shared_ptr<IFunctionHolder> ifh3 (new FunctionHolder<ft_V_V>(fp3));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(controllogicmanager_3_command),ifh3));
    devcommst.device_commands.shrink_to_fit();
    return devcommst;
}
#endif