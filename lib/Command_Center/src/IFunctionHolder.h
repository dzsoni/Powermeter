#ifndef IFUNCTIONHOLDER_H
#define IFUNCTIONHOLDER_H

#include <memory>
#include <vector>
#include <WString.h>

class IFunctionHolder
{
  public:
  IFunctionHolder() {};
  ~IFunctionHolder(){};

  virtual String run(std::vector<String>)=0;
};
#endif /* IFUNCTIONHOLDER_H */