#ifndef RELAYARRAY_COLLECTFUN_H
#define RELAYARRAY_COLLECTFUN_H

#ifndef NO_NEED_RELAYARRAY_COLLECTFUN


#include "TupleCoreFactory.h"
#include "RelayArray.h"
#include "wstring.h"
#include <memory>
#include "FunctionHolder.h"
#include "enum_CommandRepo.h"

template<> device_command_struct TupleCoreFactory::collectFuns<RelayArray>(RelayArray* relayarray,String id_by_user);
#endif
#endif