// common.h
#ifndef __COMMON__
#define __COMMON__

#include "public/common.h"


class Common : public ICommon
{
public:
  Common();
  virtual ~Common();

  void ExecuteGeneral(const char *name, const char *args);
  void ExecuteGeneral(int argc, char** argv);
  void Print(const char* fmt, ...);
};

extern Common* g_common;

#endif
