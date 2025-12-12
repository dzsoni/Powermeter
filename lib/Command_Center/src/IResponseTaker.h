#ifndef IRESPONSETAKER_H
#define IRESPONSETAKER_H

#include <WString.h>

class IResponseTaker
{
public:
    virtual void processResponse(String response)=0;
};

#endif /* IRESPONSETAKER_H */
