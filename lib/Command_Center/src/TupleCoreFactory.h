#ifndef TUPLECOREFACTORY_H
#define TUPLECOREFACTORY_H

#include "enum_CommandRepo.h"
#include <functional>
#include <vector>

//#define DEBUG_TUPLECOREF
#ifdef DEBUG_TUPLECOREF
#define _TUPLECOREF_PP(a) Serial.print(a);
#define _TUPLECOREF_PL(a) Serial.println(a);
#else
#define _TUPLECOREF_PP(a)
#define _TUPLECOREF_PL(a)
#endif

struct init_function_struct 
{
   std::function<device_command_struct(String)> _fun;
   String _id_by_user;
};

class TupleCoreFactory
{
 private:
 std::vector<init_function_struct> _funcollectinits_vec;

 public:
 TupleCoreFactory();
 ~TupleCoreFactory();
 
 device_command_struct invokeNthInitFunction(unsigned int nth,String new_id_by_user="");
 device_command_struct invokeInitFunctionById(String old_id_by_user, String new_id_by_user="");
 device_command_struct invokeInitFunctionByType(String devicetype);
 unsigned int numberOfInitFuns();
 void shrinkToFitFuncinitvec();

 template <typename E> void addDeviceInitFunction(E* instype, String id_by_user="")
 {
    if(!instype) return;
    if(id_by_user.length()==0)
    {
      id_by_user = "dev"+String(numberOfInitFuns());
    }
    init_function_struct initstruct{std::bind(&TupleCoreFactory::collectFuns<E>,this,instype,std::placeholders::_1),id_by_user};
    _funcollectinits_vec.push_back(initstruct);
 };


 template<typename T> device_command_struct collectFuns(T* t, String id_by_user){return device_command_struct();};
};

/* Specializations of the templated functions */
#include "collectFunspecdecs.h"


#endif /* TUPLECOREFACTORY_H */
