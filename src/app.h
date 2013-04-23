/**
  @file app.h
  @author Christopher Olsen
  @date Sun Jan  9 15:25:03 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __APP__
#define __APP__

class App
{
public:
  virtual ~App();
  
  static App* theApp;

  typedef int (*EntryPoint)(int, char**);
  static EntryPoint Run;

protected:
  App();
};

#endif

