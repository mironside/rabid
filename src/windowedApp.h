/**
  @file windowedApp.h
  @author Christopher Olsen
  @date Sun Jan  9 19:12:27 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __WINDOWED_APP__
#define __WINDOWED_APP__

#include "app.h"

class IRenderer;

class WindowedApp : public App
{
public:
  WindowedApp(const char* title, int x, int y,
              int w, int height, int fs);
  virtual ~WindowedApp();


  // inlines
  inline const char* GetTitle() { return m_title; }
  inline int GetXPosition()     { return m_xPosition; }
  inline int GetYPosition()     { return m_yPosition; }
  inline int GetWidth()         { return m_width; }
  inline int GetHeight()        { return m_height; }

  inline void SetXPosition(int xPos) { m_xPosition = xPos; }
  inline void SetYPosition(int yPos) { m_yPosition = yPos; }

  inline virtual bool OnPreCreate()  { return false; }
  inline virtual bool OnInitialize(int argc, char** argv) { return false; }
  inline virtual bool OnTerminate()  {return false;}
  inline virtual void OnIdle()       {}

  virtual bool OnMouseMove(int dx, int dy);

  virtual bool OnKeyPress(int key);
  virtual bool OnKeyRelease(int key);

  virtual void ReleaseMouse();
  virtual void CaptureMouse();

  virtual void Resize(int width, int height);


  // platform specific
  IRenderer* CreateRenderer(const char* API);
  void RequestTermination();
  int Main(int argc, char** argv);


protected:
  static int Run(int argc, char** argv);

private:
  const char* m_title;
  int m_xPosition, m_yPosition;
  int m_width, m_height;
  int m_fullscreen;
  int m_capturemouse;
};


#endif
