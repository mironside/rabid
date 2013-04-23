/**
  @file consoleApp.h
  @author Anthony Magro
  @date Sun Jan  9 19:12:27 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __CONSOLE_APP__
#define __CONSOLE_APP__

#include "app.h"
#include <stdio.h>


class ConsoleApp : public App
{
public:
  ConsoleApp(const char* title);
  virtual ~ConsoleApp();


  // inlines
  inline const char* GetTitle() { return m_title; }

  inline virtual bool OnPreCreate()  { return false; }
  inline virtual bool OnInitialize(int argc, char** argv) { return false; }
  inline virtual bool OnTerminate()  {return false;}
  inline virtual void OnIdle()       {}

  virtual bool OnKeyPress(int key);
  virtual bool OnKeyRelease(int key);

  virtual void Resize(int width, int height);

  virtual void AddConsoleText(const char *cmd){};

  // platform specific
  void RequestTermination();
  int Main(int argc, char** argv);
  void Print(const char* fmt, ...);


protected:
  static int Run(int argc, char** argv);

private:
  const char* m_title;
  int m_width, m_height;
};


#endif/*__CONSOLE_APP__*/
