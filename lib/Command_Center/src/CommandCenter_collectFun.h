#ifndef COMMANDCENTER_COLLECTFUN_H
#define COMMANDCENTER_COLLECTFUN_H

#ifndef  NO_NEED_COMMANDCENTER_COLLECTFUN

#include "wstring.h"
#include <memory>
#include "FunctionHolder.h"
#include "CommandCenter.h"
#include "enum_CommandRepo.h"
#include "TupleCoreFactory.h"


template<> device_command_struct TupleCoreFactory::collectFuns<CommandCenter>(CommandCenter* commcent, String id_by_user);

#endif
#endif /* COMMANDCENTER_COLLECTFUN_H */
