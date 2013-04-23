/**
  @file timer.cpp
  @author 
  @date Thu Sep  9 00:45:47 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "timer.h"
#include "common/global.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef WIN32
__int64 freq, curTime;
#endif

Timer::Timer()
{
  Reset();
}



Timer::~Timer()
{
}



void Timer::Reset()
{
  currentTime = 0.0;
  previousTime = 0.0;
  dt = 0.0;

#ifdef WIN32
  QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
  QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
  startTime = (double)curTime / (double)freq;
#else
	struct timeval  tv;
	gettimeofday(&tv, 0);
	startTime = (double)(tv.tv_sec) + ((double)(tv.tv_usec) / 1000000.0f);
#endif
}



void Timer::Update()
{
  previousTime = currentTime;

#ifdef WIN32
  QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
  currentTime = ((double)curTime / (double)freq) - startTime;
#else
  struct timeval tv;
  gettimeofday(&tv, 0);
  currentTime = (double)(tv.tv_sec) + ((double)(tv.tv_usec) / 1000000.0f) - startTime;
#endif

  dt = currentTime - previousTime;
}



float Timer::GetTime()
{
  return (float)currentTime;
}

float Timer::GetTimeChange()
{
  return (float)dt;
}


