#ifndef MYCILAPZEM_COLLECTFUN_H
#define MYCILAPZEM_COLLECTFUN_H


#include "TupleCoreFactory.h"
#include <MycilaPZEM.h>
#include "wstring.h"
#include "enum_CommandRepo.h"

// Forward declaration for MycilaPZEM wrapper
class MycilaPZEM;

template<> device_command_struct TupleCoreFactory::collectFuns<MycilaPZEM>(MycilaPZEM* pzem, String id_by_user);

#endif /* MYCILAPZEM_COLLECTFUN_H */
