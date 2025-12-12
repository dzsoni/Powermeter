#ifndef NONBLOCKINGDALLASARRAY_COLLECTFUN_H
#define NONBLOCKINGDALLASARRAY_COLLECTFUN_H

#ifndef NO_NEED_NONBLOCKINGDALLASARRAY_COLLECTFUN

#include "TupleCoreFactory.h"
#include "NonBlockingDallasArray.h"
#include "wstring.h"
#include "enum_CommandRepo.h"

template<> device_command_struct TupleCoreFactory::collectFuns<NonBlockingDallasArray>(NonBlockingDallasArray* NBDA, String id_by_user);
#endif
#endif /* NONBLOCKINGDALLASARRAY_COLLECTFUN_H */
