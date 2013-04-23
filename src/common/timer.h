/**
  @file timer.h
  @author 
  @date Thu Sep  9 00:45:58 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __TIMER__
#define __TIMER__

#include "public/timer.h"

class Timer : public ITimer
{
private:
  double startTime;
  double currentTime;
  double previousTime;
  double dt;

public:
  Timer();
  virtual ~Timer();

  float GetTime();
  
  void Reset();
  float GetTimeChange();
  void Update();
};

extern Timer g_timer;

#endif

