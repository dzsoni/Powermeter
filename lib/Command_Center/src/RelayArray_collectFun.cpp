
#include "RelayArray_collectFun.h"
#include "enum_CommandRepo.h"

#ifndef NO_NEED_RELAYARRAY_COLLECTFUN

static const char device_type_relayarray[] PROGMEM = "RELAYARRAY";

// template specialization for void CollectFuns<T>(T*)
template <> device_command_struct TupleCoreFactory::collectFuns<RelayArray>(RelayArray* relayarray, String id_by_user)
{	
    if(relayarray==nullptr) return device_command_struct();
    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_type_relayarray);
    devcommst.id_by_user=id_by_user;
    
    //bool RelayArray::addRelay(String relayid, unsigned char pin, bool initstate, bool on_state_level)
    static const char relayarray_1_command[] PROGMEM = "addRelay";
    std::shared_ptr<ft_B_S_UC_B_B> fp1 (new ft_B_S_UC_B_B(std::bind(
                                                          &RelayArray::addRelay,
                                                          relayarray,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2, 
                                                          std::placeholders::_3,
                                                          std::placeholders::_4)));
    std::shared_ptr<IFunctionHolder> ifh1 (new FunctionHolder<ft_B_S_UC_B_B>(fp1));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_1_command), ifh1));
    
    //bool RelayArray::removeRelayByPin(unsigned char pin)
    static const char relayarray_2_command[] PROGMEM = "removeRelayByPin";
    std::shared_ptr<ft_B_UC> fp2 (new ft_B_UC(std::bind(&RelayArray::removeRelayByPin, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_B_UC>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_2_command), ifh2));

    //bool RelayArray::removeRelayById(String relayid)
    static const char relayarray_3_command[] PROGMEM = "removeRelayById";
    std::shared_ptr<ft_B_S> fp3 (new ft_B_S(std::bind(&RelayArray::removeRelayById, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh3 (new FunctionHolder<ft_B_S>(fp3));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_3_command), ifh3));

    //bool RelayArray::removeRelayByIndex(unsigned char index)
    static const char relayarray_4_command[] PROGMEM = "removeRelayByIndex";
    std::shared_ptr<ft_B_UC> fp4 (new ft_B_UC(std::bind(&RelayArray::removeRelayByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh4 (new FunctionHolder<ft_B_UC>(fp4));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_4_command), ifh4));

    //bool RelayArray::changeIdByPin(unsigned char pin, String newid)
    static const char relayarray_5_command[] PROGMEM = "changeIdByPin";
    std::shared_ptr<ft_B_UC_S> fp5 (new ft_B_UC_S(std::bind(&RelayArray::changeIdByPin, relayarray, std::placeholders::_1, std::placeholders::_2)));
    std::shared_ptr<IFunctionHolder> ifh5 (new FunctionHolder<ft_B_UC_S>(fp5));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_5_command), ifh5));

    //bool RelayArray::changeIdById(String oldid,String newid)
    static const char relayarray_6_command[] PROGMEM = "changeIdById";
    std::shared_ptr<ft_B_S_S> fp6 (new ft_B_S_S(std::bind(&RelayArray::changeIdById, relayarray, std::placeholders::_1, std::placeholders::_2)));
    std::shared_ptr<IFunctionHolder> ifh6 (new FunctionHolder<ft_B_S_S>(fp6));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_6_command), ifh6));

    //bool RelayArray::changeIdByIndex(unsigned char index,String newid)
    static const char relayarray_7_command[] PROGMEM = "changeIdByIndex";
    std::shared_ptr<ft_B_UC_S> fp7 (new ft_B_UC_S(std::bind(&RelayArray::changeIdByIndex, relayarray, std::placeholders::_1, std::placeholders::_2)));
    std::shared_ptr<IFunctionHolder> ifh7 (new FunctionHolder<ft_B_UC_S>(fp7));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_7_command), ifh7));

    //bool RelayArray::initByPin(unsigned char pin, bool initstate, bool on_state_level)
    static const char relayarray_8_command[] PROGMEM = "initByPin";
    std::shared_ptr<ft_B_UC_B_B> fp8 (new ft_B_UC_B_B(std::bind(
                                                          &RelayArray::initByPin,
                                                          relayarray,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2, 
                                                          std::placeholders::_3)));
    std::shared_ptr<IFunctionHolder> ifh8 (new FunctionHolder<ft_B_UC_B_B>(fp8));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_8_command), ifh8));

    //bool RelayArray::initById(String relayid, bool initstate, bool on_state_level)
    static const char relayarray_9_command[] PROGMEM = "initById";
    std::shared_ptr<ft_B_S_B_B> fp9 (new ft_B_S_B_B(std::bind(
                                                          &RelayArray::initById,
                                                          relayarray,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2, 
                                                          std::placeholders::_3)));
    std::shared_ptr<IFunctionHolder> ifh9 (new FunctionHolder<ft_B_S_B_B>(fp9));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_9_command), ifh9));

    //bool RelayArray::initByIndex(unsigned char index, bool initstate, bool on_state_level)
    static const char relayarray_10_command[] PROGMEM = "initByIndex";
    std::shared_ptr<ft_B_UC_B_B> fp10 (new ft_B_UC_B_B(std::bind(
                                                          &RelayArray::initByIndex,
                                                          relayarray,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2, 
                                                          std::placeholders::_3)));
    std::shared_ptr<IFunctionHolder> ifh10 (new FunctionHolder<ft_B_UC_B_B>(fp10));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_10_command), ifh10));

    //void RelayArray::OnByPin(unsigned char pin)
    static const char relayarray_11_command[] PROGMEM = "OnByPin";
    std::shared_ptr<ft_V_UC> fp11 (new ft_V_UC(std::bind(&RelayArray::OnByPin, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh11 (new FunctionHolder<ft_V_UC>(fp11));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_11_command), ifh11));

    //void RelayArray::OnById(String relayid)
    static const char relayarray_12_command[] PROGMEM = "OnById";
    std::shared_ptr<ft_V_S> fp12 (new ft_V_S(std::bind(&RelayArray::OnById, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh12 (new FunctionHolder<ft_V_S>(fp12));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_12_command), ifh12));

    //void RelayArray::OnByIndex(unsigned char index)
    static const char relayarray_13_command[] PROGMEM = "OnByIndex";
    std::shared_ptr<ft_V_UC> fp13 (new ft_V_UC(std::bind(&RelayArray::OnByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh13 (new FunctionHolder<ft_V_UC>(fp13));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_13_command), ifh13));

    //void RelayArray::OffByPin(unsigned char pin)
    static const char relayarray_14_command[] PROGMEM = "OffByPin";
    std::shared_ptr<ft_V_UC> fp14 (new ft_V_UC(std::bind(&RelayArray::OffByPin, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh14 (new FunctionHolder<ft_V_UC>(fp14));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_14_command), ifh14));

    //void RelayArray::OffById(String relayid)
    static const char relayarray_15_command[] PROGMEM = "OffById";
    std::shared_ptr<ft_V_S> fp15 (new ft_V_S(std::bind(&RelayArray::OffById, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh15 (new FunctionHolder<ft_V_S>(fp15));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_15_command), ifh15));

    //void RelayArray::OffByIndex(unsigned char index)
    static const char relayarray_16_command[] PROGMEM = "OffByIndex";
    std::shared_ptr<ft_V_UC> fp16 (new ft_V_UC(std::bind(&RelayArray::OffByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh16 (new FunctionHolder<ft_V_UC>(fp16));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_16_command), ifh16));

    //void RelayArray::setStateByPin(unsigned char pin, bool state)
    static const char relayarray_17_command[] PROGMEM = "setStateByPin";
    std::shared_ptr<ft_V_UC_B> fp17 (new ft_V_UC_B(std::bind(
                                                          &RelayArray::setStateByPin,
                                                          relayarray,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2)));
    std::shared_ptr<IFunctionHolder> ifh17 (new FunctionHolder<ft_V_UC_B>(fp17));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_17_command), ifh17));
    
    //void RelayArray::setStateById(String relayid, bool state)
    static const char relayarray_18_command[] PROGMEM = "setStateById";
    std::shared_ptr<ft_V_S_B> fp18 (new ft_V_S_B(std::bind(
                                                          &RelayArray::setStateById,
                                                          relayarray,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2)));
    std::shared_ptr<IFunctionHolder> ifh18 (new FunctionHolder<ft_V_S_B>(fp18));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_18_command), ifh18));

    //void RelayArray::setStateByIndex(unsigned char index, bool state)
    static const char relayarray_19_command[] PROGMEM = "setStateByIndex";
    std::shared_ptr<ft_V_UC_B> fp19 (new ft_V_UC_B(std::bind(
                                                          &RelayArray::setStateByIndex,
                                                          relayarray,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2)));
    std::shared_ptr<IFunctionHolder> ifh19 (new FunctionHolder<ft_V_UC_B>(fp19));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_19_command), ifh19));

    //bool RelayArray::getStateByPin(unsigned char pin)
    static const char relayarray_20_command[] PROGMEM = "getStateByPin";
    std::shared_ptr<ft_B_UC> fp20 (new ft_B_UC(std::bind(&RelayArray::getStateByPin, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh20 (new FunctionHolder<ft_B_UC>(fp20));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_20_command), ifh20));

    //bool RelayArray::getStateById(String relayid)
    static const char relayarray_21_command[] PROGMEM = "getStateById";
    std::shared_ptr<ft_B_S> fp21 (new ft_B_S(std::bind(&RelayArray::getStateById, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh21 (new FunctionHolder<ft_B_S>(fp21));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_21_command), ifh21));

    //bool RelayArray::getStateByIndex(unsigned char index)
    static const char relayarray_22_command[] PROGMEM = "getStateByIndex";
    std::shared_ptr<ft_B_UC> fp22 (new ft_B_UC(std::bind(&RelayArray::getStateByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh22 (new FunctionHolder<ft_B_UC>(fp22));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_22_command), ifh22));

    //bool RelayArray::getInitStateByPin(unsigned char pin)
    static const char relayarray_23_command[] PROGMEM = "getInitStateByPin";
    std::shared_ptr<ft_B_UC> fp23 (new ft_B_UC(std::bind(&RelayArray::getInitStateByPin, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh23 (new FunctionHolder<ft_B_UC>(fp23));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_23_command), ifh23));

    //bool RelayArray::getInitStateById(String relayid)
    static const char relayarray_24_command[] PROGMEM = "getInitStateById";
    std::shared_ptr<ft_B_S> fp24 (new ft_B_S(std::bind(&RelayArray::getInitStateById, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh24 (new FunctionHolder<ft_B_S>(fp24));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_24_command), ifh24));
        
    //bool RelayArray::getInitStateByIndex(unsigned char index)
    static const char relayarray_25_command[] PROGMEM = "getInitStateByIndex";
    std::shared_ptr<ft_B_UC> fp25 (new ft_B_UC(std::bind(&RelayArray::getInitStateByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh25 (new FunctionHolder<ft_B_UC>(fp25));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_25_command), ifh25));

    //bool RelayArray::getOnStateLevelByPin(unsigned char pin)
    static const char relayarray_26_command[] PROGMEM = "getOnStateLevelByPin";
    std::shared_ptr<ft_B_UC> fp26 (new ft_B_UC(std::bind(&RelayArray::getOnStateLevelByPin, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh26 (new FunctionHolder<ft_B_UC>(fp26));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_26_command), ifh26));

    //bool RelayArray::getOnStateLevelById(String relayid)
    static const char relayarray_27_command[] PROGMEM = "getOnStateLevelById";
    std::shared_ptr<ft_B_S> fp27 (new ft_B_S(std::bind(&RelayArray::getOnStateLevelById, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh27 (new FunctionHolder<ft_B_S>(fp27));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_27_command), ifh27));

    //bool RelayArray::getOnStateLevelByIndex(unsigned char index)
    static const char relayarray_28_command[] PROGMEM = "getOnStateLevelByIndex";
    std::shared_ptr<ft_B_UC> fp28 (new ft_B_UC(std::bind(&RelayArray::getOnStateLevelByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh28 (new FunctionHolder<ft_B_UC>(fp28));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_28_command), ifh28));

    //int RelayArray::getRelayPinById(String relayid)
    static const char relayarray_29_command[] PROGMEM = "getRelayPinById";
    std::shared_ptr<ft_I_S> fp29 (new ft_I_S(std::bind(&RelayArray::getRelayPinById, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh29 (new FunctionHolder<ft_I_S>(fp29));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_29_command), ifh29));

    //int RelayArray::getRelayPinByIndex(unsigned char index)
    static const char relayarray_30_command[] PROGMEM = "getRelayPinByIndex";
    std::shared_ptr<ft_I_UC> fp30 (new ft_I_UC(std::bind(&RelayArray::getRelayPinByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh30 (new FunctionHolder<ft_I_UC>(fp30));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_30_command), ifh30));

    //String RelayArray::getRelayIdByPin(unsigned char pin)
    static const char relayarray_31_command[] PROGMEM = "getRelayIdByPin";
    std::shared_ptr<ft_S_UC> fp31 (new ft_S_UC(std::bind(&RelayArray::getRelayIdByPin, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh31 (new FunctionHolder<ft_S_UC>(fp31));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_31_command), ifh31));

    //String RelayArray::getRelayIdByIndex(unsigned char index)
    static const char relayarray_32_command[] PROGMEM = "getRelayIdByIndex";
    std::shared_ptr<ft_S_UC> fp32 (new ft_S_UC(std::bind(&RelayArray::getRelayIdByIndex, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh32 (new FunctionHolder<ft_S_UC>(fp32));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_32_command), ifh32));

    //int RelayArray::getRelayCount()
    static const char relayarray_33_command[] PROGMEM = "getRelayCount";
    std::shared_ptr<ft_I_V> fp33 (new ft_I_V(std::bind(&RelayArray::getRelayCount, relayarray)));
    std::shared_ptr<IFunctionHolder> ifh33 (new FunctionHolder<ft_I_V>(fp33));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_33_command), ifh33));

    //bool RelayArray::isIdExist(String relayid)
    static const char relayarray_34_command[] PROGMEM = "isIdExist";
    std::shared_ptr<ft_B_S> fp34 (new ft_B_S(std::bind(&RelayArray::isIdExist, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh34 (new FunctionHolder<ft_B_S>(fp34));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_34_command), ifh34));

    //void RelayArray::shrinkToFit()
    static const char relayarray_35_command[] PROGMEM = "shrinkToFit";
    std::shared_ptr<ft_V_V> fp35 (new ft_V_V(std::bind(&RelayArray::shrinkToFit, relayarray)));
    std::shared_ptr<IFunctionHolder> ifh35 (new FunctionHolder<ft_V_V>(fp35));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_35_command), ifh35));

    //void RelayArray::loadRelays(String pathtoJsonfile)
    static const char relayarray_36_command[] PROGMEM = "loadRelays";
    std::shared_ptr<ft_V_S> fp36 (new ft_V_S(std::bind(&RelayArray::loadRelays, relayarray, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh36 (new FunctionHolder<ft_V_S>(fp36));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_36_command), ifh36));

    //void RelayArray::saveRelays(String pathtoJsonfile)
    static const char relayarray_37_command[] PROGMEM = "saveRelays";    
    std::shared_ptr<ft_V_S> fp37 (new ft_V_S(std::bind(&RelayArray::saveRelays, relayarray, std::placeholders::_1)));    
    std::shared_ptr<IFunctionHolder> ifh37 (new FunctionHolder<ft_V_S>(fp37));    
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(relayarray_37_command), ifh37));
    
    devcommst.device_commands.shrink_to_fit();
    return devcommst;
}
#endif

