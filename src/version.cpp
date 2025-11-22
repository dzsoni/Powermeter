#include <WString.h>
#include "version.h"

String getVersion()
{
    return String(String("v")+ String(GIT_TAG) + " - " + String(GIT_BRANCH));
}
