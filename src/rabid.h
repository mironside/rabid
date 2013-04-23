/**
  @file rabid.h
  @author Christopher Olsen
  @date Fri Mar 25 02:18:04 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __RABID__
#define __RABID__

#include "windowedApp.h"

class RabidEngine : public WindowedApp
{
public:
  RabidEngine();
  ~RabidEngine();

  static int initialized;
  static int Initialize();

  virtual void Resize(int width, int height);

  bool OnInitialize(int argc, char** argv);
  void OnIdle();
  bool OnTerminate();
  bool OnKeyPress(int key);
  bool OnKeyRelease(int key);
  bool OnMouseMove(int dx, int dy);
  int LoadMap(const char* filename);
  void ParseArgs(int argc, char** argv);
};

#endif
