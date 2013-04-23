/**
  @file app.cpp
  @author Christopher Olsen
  @date Sun Jan  9 15:29:08 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "app.h"
#include <stdio.h>
#include "common/global.h"

#ifdef LINUX
#include <mcheck.h>
#endif

App::EntryPoint App::Run = 0;
App* App::theApp = 0;

App::App()
{
}

App::~App()
{
}


int main(int argc, char** argv)
{
#ifdef LINUX
  mtrace();
#endif
  int rCode = -1;
  if(App::Run)
  {
    rCode = App::Run(argc, argv);
  }


  delete App::theApp;
  App::theApp = 0;
#ifdef LINUX
  muntrace();
#endif
  return rCode;
}
