#ifndef WIFIMANAGER_COLLECTFUN_H
#define WIFIMANAGER_COLLECTFUN_H

#ifndef NO_NEED_WIFIMANAGER_COLLECTFUN

#include "WifiManager.h"
#include "TupleCoreFactory.h"
#include "wstring.h"
#include <memory>
#include "FunctionHolder.h"


template<> device_command_struct TupleCoreFactory::collectFuns<WifiManager>(WifiManager* cl, String id_by_user);


#endif
#endif /* WIFIMANAGER_COLLECTFUN_H */
