/**
  @file cvarSystem.h
  @author Christopher Olsen
  @date Thu Dec 16 03:18:35 2004


*/
#ifndef __ICVAR_SYSTEM__
#define __ICVAR_SYSTEM__

#include <stdlib.h>
#include <string.h>
#include <memory.h>


#ifdef WIN32
#define INLINE __forceinline
#else
#define INLINE inline
#endif

void *operator new(size_t s, const char *filename, unsigned long line);
void *operator new[](size_t s, const char *filename, unsigned long line);

class CVar
{
protected:
  const char* name;
  char* value;
  float floatVal;
  int intVal;
  CVar* next;
  static CVar* staticVars;
  bool changed;

public:
  CVar(const char* cvarName, char* cvarValue) : name(0)
  { Initialize(cvarName, cvarValue); }
  virtual ~CVar() { delete value; };

  void Initialize(const char* cvarName, const char* cvarValue);

  const char* GetName() { return name; }
  const char* GetString() { return value; }
  int GetInt() { return intVal; }
  bool GetBool() { return intVal != 0; }
  float GetFloat() { return floatVal; }
  void SetFloat(const float val)
  { floatVal = val; }
  bool Changed() { bool t = changed; changed = false; return t; }
    
  void Set(const char* cvarValue)
  {
    if(!cvarValue)
      return;

    if(value)
      delete[] value;

    value = new char[strlen(cvarValue)+1];
    memset(value, 0, strlen(cvarValue)+1);
    strcpy(value, cvarValue);

    floatVal = (float)atof(cvarValue);
    intVal = atoi(cvarValue);
    changed = true;
  }
  static void RegisterStaticCVars();
};


class ICVarSystem
{
public:
  virtual ~ICVarSystem() {};
  virtual int Initialize() = 0;
  virtual void Release() = 0;
  virtual void Register(CVar* cvar) = 0;
  virtual CVar* Find(const char* name) = 0;
  virtual const char* Complete(const char* cvar) = 0;
};

extern ICVarSystem* g_cvarSystem;


INLINE void CVar::Initialize(const char* cvarName, const char* cvarValue)
{
  this->name  = cvarName;
  Set(cvarValue);

  if(g_cvarSystem)
    g_cvarSystem->Register(this);
  else
  {
    this->next = staticVars;
    staticVars = this;
  }
}


INLINE void CVar::RegisterStaticCVars()
{
  if(g_cvarSystem)
  {
    for(CVar* cvar = staticVars; cvar; cvar = cvar->next)
      g_cvarSystem->Register(cvar);
  }
}

#endif

