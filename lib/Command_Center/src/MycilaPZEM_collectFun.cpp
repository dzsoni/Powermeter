#include "MycilaPZEM_collectFun.h"

#ifndef NO_NEED_MYCILAPZEM_COLLECTFUN

static const char device_type_mycilaPZEM[] PROGMEM = "MYCILAPZEM";
// template specialization for void CollectFuns<T>(T*)
template<> device_command_struct TupleCoreFactory::collectFuns<MycilaPZEM>(MycilaPZEM* pzem, String id_by_user)
{
    if(pzem==nullptr) return device_command_struct();
    return device_command_struct (); //empty  
}
#endif