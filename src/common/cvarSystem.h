/**
  @file cvarSystem.h
  @author Christopher Olsen
  @date Thu Dec 16 03:18:35 2004


*/
#ifndef __CVAR_SYSTEM__
#define __CVAR_SYSTEM__

#include "public/cvarSystem.h"
#include "libsn/hashMap.h"




class CVarSystem : public ICVarSystem
{
protected:
  snSHashMap<CVar*> cvars;

public:
  CVarSystem();
  virtual ~CVarSystem();

  int Initialize();
  void Release();
  void Print(const char* name);
  void Set(const char* name, const char* args);
  CVar* Find(const char* name);
  const char* Complete(const char*);

  void Register(CVar* cvar);
};

#endif

