/**
  @file timer.h
  @author 
  @date Thu Sep  9 00:45:58 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __ITIMER__
#define __ITIMER__

class ITimer
{
public:

  virtual void Reset() = 0;
  virtual float GetTimeChange() = 0;
  virtual float GetTime() = 0;
};

#ifdef GAME
extern ITimer* g_timer;
#endif

#endif

