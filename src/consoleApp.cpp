/**
  @file windowedApp.cpp
  @author Christopher Olsen
  @date Sun Jan  9 19:13:48 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "consoleApp.h"
#include <string.h>
#include <stdio.h>
#include "common/keys.h"
#include "common/global.h"


ConsoleApp::ConsoleApp(const char* title)
{
}



ConsoleApp::~ConsoleApp()
{
}



int ConsoleApp::Run(int argc, char** argv)
{
  ConsoleApp* app = (ConsoleApp*)theApp;
  return app->Main(argc, argv);
}



bool ConsoleApp::OnKeyPress(int key)
{
  if(key == K_ESCAPE)
  {
    RequestTermination();
    return true;
  }

  return false;
}



bool ConsoleApp::OnKeyRelease(int key)
{
  return false;
}


void ConsoleApp::Resize(int newWidth, int newHeight)
{
  m_width = newWidth;
  m_height = newHeight;
}


