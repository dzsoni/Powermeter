#include "TupleCoreFactory.h"

TupleCoreFactory::TupleCoreFactory()
{
}

TupleCoreFactory::~TupleCoreFactory()
{
}

/**
 * \brief Invoke the nth initialization function from the stored vector of them.
 *
 * If id_by_user is not empty, the function will set the id_by_user of the nth
 * function to that value. Otherwise, it will take the id_by_user from the
 * stored struct.
 *
 * \param nth The 0-based index of the initialization function to invoke.
 * \param id_by_user The id_by_user to set for the nth function. If empty, the
 * one from the stored struct is taken.
 * \return The result of the invoked function,wich is a device_command_struct.
 */
device_command_struct TupleCoreFactory::invokeNthInitFunction(unsigned int nth,String new_id_by_user)
{
   
   if(nth>=_funcollectinits_vec.size()) return device_command_struct();
    _funcollectinits_vec[nth]._id_by_user= (new_id_by_user.length()>0)?new_id_by_user:_funcollectinits_vec[nth]._id_by_user; 
   return _funcollectinits_vec[nth]._fun(_funcollectinits_vec[nth]._id_by_user);    
}

device_command_struct TupleCoreFactory::invokeInitFunctionById(String old_id_by_user,String new_id_by_user)
{
    for(size_t i=0;i<_funcollectinits_vec.size();i++)
    { 
        if(_funcollectinits_vec[i]._id_by_user==new_id_by_user)
        {
            Serial.println("Error: Same id.");
            return device_command_struct();
        }
        
    }
    for(size_t i=0;i<_funcollectinits_vec.size();i++)
    { 
        if(_funcollectinits_vec[i]._id_by_user==old_id_by_user)
        { 
            _funcollectinits_vec[i]._id_by_user= (new_id_by_user.length()>0)?new_id_by_user:_funcollectinits_vec[i]._id_by_user; 
            return _funcollectinits_vec[i]._fun(_funcollectinits_vec[i]._id_by_user);
        }
    }
    return device_command_struct();
}

device_command_struct TupleCoreFactory::invokeInitFunctionByType(String devicetype)
{
    for(size_t i=0;i<_funcollectinits_vec.size();i++)
    { 
        device_command_struct dcs = _funcollectinits_vec[i]._fun(_funcollectinits_vec[i]._id_by_user);
        if(String(dcs.devicetype)==devicetype)
        {   
            return _funcollectinits_vec[i]._fun(_funcollectinits_vec[i]._id_by_user);
        }
    }
    return device_command_struct();
}

uint TupleCoreFactory::numberOfInitFuns()
{
    return (uint)_funcollectinits_vec.size();
}

void TupleCoreFactory::shrinkToFitFuncinitvec()
{
    _funcollectinits_vec.shrink_to_fit();
}
