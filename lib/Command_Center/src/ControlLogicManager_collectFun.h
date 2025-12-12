#ifndef CONTROLLOGICMANAGER_COLLECTFUN_H
#define CONTROLLOGICMANAGER_COLLECTFUN_H

#ifndef  NO_NEED_CONTROLLOGICMANAGER_COLLECTFUN

#include "wstring.h"
#include "enum_CommandRepo.h"
#include "TupleCoreFactory.h"
#include "ControlLogicManager.h"

template<> device_command_struct TupleCoreFactory::collectFuns<ControlLogicManager>(ControlLogicManager* CLM, String id_by_user);

#endif
#endif /* CONTROLLOGICMANAGER_COLLECTFUN_H */
