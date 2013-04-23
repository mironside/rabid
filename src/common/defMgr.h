/**
  @file defMgr.h
  @author Christopher Olsen
  @date Wed Dec 22 22:24:49 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __DEF_MGR__
#define __DEF_MGR__

#include "public/defMgr.h"
#include "libsn/hashMap.h"

class IDef;
class Def;

class DefMgr : public IDefMgr
{
protected:
  snSHashMap<Def*> defs;

public:
  DefMgr() {}
  virtual ~DefMgr();

  IDef* GetDef(const char* name);
  static void GetName(const char* file, const char* objName,
                      const char* ext, char* name);
  int GetNumObjects();
};

extern DefMgr* g_defMgr;

#endif

