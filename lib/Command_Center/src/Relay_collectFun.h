#ifndef RELAY_COLLECTFUN_H
#define RELAY_COLLECTFUN_H

#ifndef NO_NEED_RELAY_COLLECTFUN

#include "TupleCoreFactory.h"
#include "Relay.h"
#include "wstring.h"
#include <memory>
#include "FunctionHolder.h"
#include "enum_CommandRepo.h"

template<> device_command_struct TupleCoreFactory::collectFuns<Relay>(Relay* relay,String id_by_user);

#endif
#endif /* RELAY_COLLECTFUN_H */
