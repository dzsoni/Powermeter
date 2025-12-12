#ifndef COUNTER_COLLECTFUN_H
#define COUNTER_COLLECTFUN_H

#ifndef NO_NEED_COUNTER_COLLECTFUN

#include "TupleCoreFactory.h"
#include "Counter.h"
#include "wstring.h"
#include <memory>
#include "FunctionHolder.h"
#include "enum_CommandRepo.h"

template<> device_command_struct TupleCoreFactory::collectFuns<Counter>(Counter* counter,String id_by_user);

#endif
#endif /* COUNTER_COLLECTFUN_H */
