#include  "NonBlockingDallasArray_collectFun.h"
#include  "enum_CommandRepo.h"

#ifndef NO_NEED_NONBLOCKINGDALLASARRAY_COLLECTFUN

// template specialization for void CollectFuns<T>(T*)
static const char device_type_NDBArray[] PROGMEM = "NBDARRAY";

template<> device_command_struct TupleCoreFactory::collectFuns<NonBlockingDallasArray>(NonBlockingDallasArray *NBDA, String id_by_user)
{	
    if(NBDA==nullptr) return device_command_struct();
    device_command_struct devcommst;
    devcommst.devicetype=FPSTR(device_type_NDBArray);
    devcommst.id_by_user=id_by_user;

    //float struct_hardwares::getTempByNameS(String)
    static const char nbdarray_command1[] PROGMEM = "getTempByNameS";
    std::shared_ptr<ft_F_S> fp1(new ft_F_S(std::bind(&NonBlockingDallasArray::getTempByNameS, NBDA, std::placeholders::_1)));
    std::shared_ptr<IFunctionHolder> ifh1 (new FunctionHolder<ft_F_S>(fp1));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(nbdarray_command1), ifh1));

    static const char nbdarray_command2[] PROGMEM = "getTotalCountOfSensors";
    std::shared_ptr<ft_UI_V> fp2(new ft_UI_V(std::bind(&NonBlockingDallasArray::getSensorsCount, NBDA)));
    std::shared_ptr<IFunctionHolder> ifh2 (new FunctionHolder<ft_UI_V>(fp2));
    devcommst.device_commands.emplace_back(std::make_tuple(FPSTR(nbdarray_command2), ifh2));
    devcommst.device_commands.shrink_to_fit();

    return devcommst;
}
#endif