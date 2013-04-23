/**
  @file defMgr.h
  @author Christopher Olsen
  @date Wed Dec 22 22:37:44 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __IDEF_MGR__
#define __IDEF_MGR__

class IDef;
class Object;

class IDefMgr
{
public:
  virtual ~IDefMgr() {}
  virtual IDef* GetDef(const char* name) = 0;
  virtual int GetNumObjects() = 0;
};

#ifdef GAME
extern IDefMgr* g_defMgr;
#endif


#endif

