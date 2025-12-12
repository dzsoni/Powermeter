#include "WifiManager_collectFun.h"
#ifndef NO_NEED_WIFIMANAGER_COLLECTFUN
static const char device_wifi_manager[] PROGMEM = "WIFIMANAGER";

template<> device_command_struct TupleCoreFactory::collectFuns<WifiManager>(WifiManager* cl, String id_by_user)
{
  if(cl==nullptr) return device_command_struct();
  device_command_struct devcommst;
  devcommst.devicetype=FPSTR(device_wifi_manager);
  devcommst.id_by_user=id_by_user;

  //void WifiManager::begin()
    static const char wifimanager_1_command[] PROGMEM = "begin";
    std::shared_ptr<ft_V_B> fp1( new ft_V_B(std::bind(&WifiManager::begin, cl)));
    std::shared_ptr<IFunctionHolder> ifh1 (new FunctionHolder<ft_V_B>(fp1));
    devcommst.device_commands.emplace_back(std::make_tuple( FPSTR(wifimanager_1_command),ifh1));

    //void WifiManager::disconnect(bool wifioff=false)
    static const char wifimanager_2_command[] PROGMEM = "disconnect";
    std::shared_ptr<ft_V_V> fp2( new ft_V_V(std::bind(&WifiManager::disconnect, cl)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_V_V>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(wifimanager_2_command),ifh2));

    //void WifiManager::addCredential(String, String, uint8_t index)
    static const char wifimanager_3_command[] PROGMEM = "addCredentials";
    std::shared_ptr<ft_V_S_S_UC> fp3( new ft_V_S_S_UC(std::bind(&WifiManager::addCredentials, cl, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    std::shared_ptr<IFunctionHolder> ifh3 (new FunctionHolder<ft_V_S_S_UC>(fp3));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(wifimanager_3_command),ifh3));

    //void WifiManager::setPathToSecretjson(String)
    static const char wifimanager_4_command[] PROGMEM = "setPathToSecretjson";
    std::shared_ptr<ft_V_S> fp4( new ft_V_S(std::bind(&WifiManager::setPathToSecretjson, cl, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh4 (new FunctionHolder<ft_V_S>(fp4));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(wifimanager_4_command),ifh4));

    //void loadCredentials()
    static const char wifimanager_5_command[] PROGMEM = "loadCredentials";
    std::shared_ptr<ft_V_V> fp5( new ft_V_V(std::bind(&WifiManager::loadCredentials, cl)));
    std::shared_ptr<IFunctionHolder> ifh5 (new FunctionHolder<ft_V_V>(fp5));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(wifimanager_5_command),ifh5));

    //void saveCredentials()
    static const char wifimanager_6_command[] PROGMEM = "saveCredentials";
    std::shared_ptr<ft_V_V> fp6( new ft_V_V(std::bind(&WifiManager::saveCredentials, cl)));
    std::shared_ptr<IFunctionHolder> ifh6 (new FunctionHolder<ft_V_V>(fp6));
    devcommst.device_commands.emplace_back(std::make_tuple( FPSTR(wifimanager_6_command),ifh6));

    //void setAPpassword(String pass)
    static const char wifimanager_7_command[] PROGMEM = "setAPpassword";
    std::shared_ptr<ft_V_S> fp7( new ft_V_S(std::bind(&WifiManager::setAPpassword, cl, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh7 (new FunctionHolder<ft_V_S>(fp7));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(wifimanager_7_command),ifh7));
    devcommst.device_commands.shrink_to_fit();
    return devcommst;
}
#endif