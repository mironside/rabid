/**
  @file cvarSystem.cpp
  @author Christopher Olsen
  @date Thu Dec 16 03:29:30 2004


*/
#include "common/cvarSystem.h"
#include "common/log.h"
#include "common/common.h"
#include "libsn/vector.h"
#include "common/global.h"

CVar* CVar::staticVars;


CVarSystem::CVarSystem()
{
}



CVarSystem::~CVarSystem()
{
  snVector<const char*> keys = cvars.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
    delete cvars[keys[i]];
}



int CVarSystem::Initialize()
{
  return 1;
}



void CVarSystem::Release()
{
}



void CVarSystem::Register(CVar* cvar)
{
  if(!cvars.Exists(cvar->GetName()))
    cvars[cvar->GetName()] = cvar;
}


void CVarSystem::Print(const char* name)
{
  CVar* cvar = Find(name);
  if(cvar)
    g_common->Print("%s = %s", name, cvar->GetString());
}


void CVarSystem::Set(const char* name, const char* args)
{
  CVar* cvar = Find(name);
  if(cvar)
    cvar->Set(args);
}


CVar* CVarSystem::Find(const char* name)
{
  if(cvars.Exists(name))
    return cvars[name];
  return 0;
}


const char* CVarSystem::Complete(const char* cvar)
{
  snVector<const char*> vars;
  snVector<const char*> keys = cvars.GetKeys();

  for(unsigned int i = 0; i < keys.Size(); i++)
  {
    if(strncmp(cvar, keys[i], strlen(cvar)) == 0)
      vars.PushBack(keys[i]);
  }

  if(vars.Size() == 1)
  {
    return vars[0];
  }
  else
  {
    for(unsigned int i = 0; i < vars.Size(); i++)
      g_common->Print("%s", vars[i]);
  }

  return 0;
}
