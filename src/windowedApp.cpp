/**
  @file windowedApp.cpp
  @author Christopher Olsen
  @date Sun Jan  9 19:13:48 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "windowedApp.h"
#include <string.h>
#include <stdio.h>
#include "common/keys.h"
#include "common/global.h"


WindowedApp::WindowedApp(const char* title, int x, int y,
                         int width, int height, int fs) :
  App(), m_title(title), m_xPosition(x), m_yPosition(y),
  m_width(width), m_height(height), m_fullscreen(fs), m_capturemouse(0)
{
}



WindowedApp::~WindowedApp()
{
}



int WindowedApp::Run(int argc, char** argv)
{
  WindowedApp* app = (WindowedApp*)theApp;
  return app->Main(argc, argv);
}



bool WindowedApp::OnKeyPress(int key)
{
  if(key == K_ESCAPE)
  {
    RequestTermination();
    return true;
  }

  return false;
}



bool WindowedApp::OnKeyRelease(int key)
{
  return false;
}


bool WindowedApp::OnMouseMove(int dx, int dy)
{
  return false;
}

void WindowedApp::Resize(int newWidth, int newHeight)
{
  m_width = newWidth;
  m_height = newHeight;
}


