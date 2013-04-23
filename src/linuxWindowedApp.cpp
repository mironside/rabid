 /**
  @file linuxWindowedApp.cpp
  @author Christopher Olsen
  @date Sun Jan  9 19:38:26 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "windowedApp.h"
#include <stdio.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include "common/keys.h"
#include "render/renderer.h"


static Display* display = 0;
static Window   window = 0;
static int      screen = 0;
static XF86VidModeModeInfo **modes;
static int mouseCaptured = 0;
static bool runApp = true;

typedef struct keymap_s
{
  int osKey;
  int appKey;
} keymap_t;


// X keymapping
static keymap_t keyMap[] =
{
  {XK_Escape,    K_ESCAPE},
  {XK_Left,      K_LEFT},
  {XK_Right,     K_RIGHT},
  {XK_Up,        K_UP},
  {XK_Down,      K_DOWN},
  {XK_Home,      K_HOME},
  {XK_End,       K_END},
  {XK_Page_Up,   K_PGUP},
  {XK_Page_Down, K_PGDN},
  {XK_Insert,    K_INS},
  {XK_Delete,    K_DEL},
  {XK_F1,        K_F1},
  {XK_F2,        K_F2},
  {XK_F3,        K_F3},
  {XK_F4,        K_F4},
  {XK_F5,        K_F5},
  {XK_F6,        K_F6},
  {XK_F7,        K_F7},
  {XK_F8,        K_F8},
  {XK_F9,        K_F9},
  {XK_F10,       K_F10},
  {XK_F11,       K_F11},
  {XK_F12,       K_F12},

  {XK_Shift_L,   K_LSHIFT},
  {XK_Shift_R,   K_RSHIFT},
  {XK_Control_L, K_LCONTROL},
  {XK_Control_R, K_RCONTROL},
  {XK_Alt_L,     K_LALT},
  {XK_Alt_R,     K_RALT},
  {XK_Caps_Lock, K_CAPS},
  {XK_Return,    K_ENTER},
  {XK_BackSpace, K_BACKSPACE},
  {XK_space,     K_SPACE},


  {XK_Tab,         K_TAB},
  {XK_Print,       K_PRINTSCREEN},
  {XK_Scroll_Lock, K_SCRLK},
  {XK_Pause,       K_PAUSE},

  {XK_Num_Lock,     K_NUMLK},
  {XK_KP_Divide,    KP_SLASH},
  {XK_KP_Multiply,  KP_MUL},
  {XK_KP_Subtract,  KP_MINUS},
  {XK_KP_Home,      KP_HOME},
  {XK_KP_Up,        KP_UP},
  {XK_KP_Page_Up,   KP_PGUP},
  {XK_KP_Left,      KP_LEFT},
  {XK_KP_Begin,     KP_5},
  {XK_KP_Right,     KP_RIGHT},
  {XK_KP_End,       KP_END},
  {XK_KP_Down,      KP_DOWN},
  {XK_KP_Page_Down, KP_PGDN},
  {XK_KP_Insert,    KP_INS},
  {XK_KP_Delete,    KP_DEL},
  {XK_KP_Enter,     KP_ENTER},
  {XK_KP_Add,       KP_PLUS},

  {-1, -1}
};

int os_to_app(int key)
{
  if(key >= ' ' && key <= 127)
    return key;

  // search mapping
  for(int i = 0; keyMap[i].osKey >= 0; i++)
  {
    if(keyMap[i].osKey == key)
      return keyMap[i].appKey;
  }

  return -1;
}


bool KeyRepeat(Display *display, XEvent &event)
{
  XEvent peek;
  bool repeat = false;
  
  // See if there are any events
  if(XPending(display)) 
  {
    // Get the first event in the queue
    XPeekEvent(display, &peek);
    // See if it is a KeyPress for our KeyRelease and match the timestamp
    if((peek.type == KeyPress) &&
       (peek.xkey.keycode == event.xkey.keycode) &&
       ((peek.xkey.time - event.xkey.time) <= 1)) 
    {
      // Our KeyRelease is a repeat
      repeat = true;
      // remove KeyPress from Queue
//      XNextEvent(display, &peek);
    }
  }

  return repeat;
}

int XWindowProc(Display* display, Window& window, XEvent& event)
{
  WindowedApp* theApp = (WindowedApp*)App::theApp;

  switch(event.type)
  {
  case ConfigureNotify:
    theApp->Resize(event.xconfigure.width, event.xconfigure.height);
    break;

  case DestroyNotify:
    return false;
    break;

  case KeyRelease:
    {
      int keysym = os_to_app(XLookupKeysym(&event.xkey, 0));
      
      if(KeyRepeat(display, event)) {
        break;
      }
      theApp->OnKeyRelease(keysym);
    }
    break;

  case KeyPress:
    {
      int keysym = os_to_app(XLookupKeysym(&event.xkey, 0));
      theApp->OnKeyPress(keysym);
    }
    break;

  case ButtonPress:
    {
      int button = event.xbutton.button;
      if(button == 1)
        theApp->OnKeyPress(M_LEFT);
      else if(button == 2)
        theApp->OnKeyPress(M_MIDDLE);
      else if(button == 3)
        theApp->OnKeyPress(M_RIGHT);
      else if(button == 4)
        theApp->OnKeyPress(M_UP);
      else if(button == 5)
        theApp->OnKeyPress(M_DOWN);
    }
    break;

  case ButtonRelease:
    {
      int button = event.xbutton.button;
      if(button == 1)
        theApp->OnKeyRelease(M_LEFT);
      else if(button == 2)
        theApp->OnKeyRelease(M_MIDDLE);
      else if(button == 3)
        theApp->OnKeyRelease(M_RIGHT);
      else if(button == 4)
        theApp->OnKeyRelease(M_UP);
      else if(button == 5)
        theApp->OnKeyRelease(M_DOWN);
    }
    break;

  case MotionNotify:
    {
      if(mouseCaptured)
      {
        static int oldmx = 320;
        static int oldmy = 240;
        const int edge = 64;
        int deltax = event.xmotion.x - oldmx;
        int deltay = event.xmotion.y - oldmy;
        theApp->OnMouseMove(deltax, deltay);
      
        oldmx = event.xmotion.x;
        oldmy = event.xmotion.y;

        if((event.xmotion.x < edge) ||
           (event.xmotion.x > (theApp->GetWidth() - edge)) ||
           (event.xmotion.y < edge) ||
           (event.xmotion.y > (theApp->GetHeight() - edge)) )
        {
          // Get the events that have accumulated 
          while(XCheckTypedEvent(display, MotionNotify, &event))
          {
            deltax = event.xmotion.x - oldmx;
            deltay = event.xmotion.y - oldmy;

            oldmx = event.xmotion.x;
            oldmy = event.xmotion.y;
          }

          oldmx = theApp->GetWidth() / 2;
          oldmy = theApp->GetHeight() / 2;

          XWarpPointer(display, None, window, 0, 0, 0, 0, oldmx, oldmy);

          for(int i=0; i<10; ++i)
          {
            XMaskEvent(display, PointerMotionMask, &event);
            if ((event.xmotion.x > (oldmx - edge)) &&
                (event.xmotion.x < (oldmx + edge)) &&
                (event.xmotion.y > (oldmy - edge)) &&
                (event.xmotion.y < (oldmy + edge)) )
            {
              break;
            }      
          }
        }
      }
    }
    break;
      
  default:
    break;
  }

  return true;
}


void WindowedApp::RequestTermination()
{
  printf("Requesting Termination\n");
  runApp = false;
  //XDestroyWindow(display, window);
}


void CreateWindow(const char* title, int& x, int& y,
                  int& width, int& height, int& fullscreen)
{
  int i;
  int vidModeMajorVersion, vidModeMinorVersion;
  int numModes;
  int bestMode;
  Window winDummy;
  unsigned int borderDummy;
  XSetWindowAttributes attr;
  XF86VidModeModeInfo deskMode;

  /* set best mode to current */
  bestMode = 0;

  /* get a connection */
  display = XOpenDisplay(0);
  screen = DefaultScreen(display);
  XF86VidModeQueryVersion(display, &vidModeMajorVersion, &vidModeMinorVersion);
  XF86VidModeGetAllModeLines(display, screen, &numModes, &modes);

  /* save desktop-resolution before switching modes */
  deskMode = *modes[0];

  /* look for mode with requested resolution */
  for (i = 0; i < numModes; i++)
  {
    if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
    {
      bestMode = i;
    }
  }


  width      = modes[bestMode]->hdisplay;
  height     = modes[bestMode]->vdisplay;
  int depth  = 32;


  if(fullscreen)
  {
    XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
    XF86VidModeSetViewPort(display, screen, 0, 0);
    XFree(modes);

    attr.override_redirect = True;
    attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask |
                      StructureNotifyMask | KeyReleaseMask | PointerMotionMask;
    window = XCreateWindow(display, RootWindow(display, screen),
                           0,0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent,
                           CWEventMask | CWOverrideRedirect, &attr);
    
    XWarpPointer(display, None, window, 0, 0, 0, 0, 0, 0);
    XMapRaised(display, window);
    XGrabKeyboard(display, window, True, GrabModeAsync,
                  GrabModeAsync, CurrentTime);
    XGrabPointer(display, window, True, ButtonPressMask,
                 GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
  }
  else
  {
    // create a window in window mode
    attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask |
                      StructureNotifyMask | KeyReleaseMask | PointerMotionMask;
    window = XCreateWindow(display, RootWindow(display, screen),
                           0,0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent,
                           CWEventMask, &attr);
    XMapRaised(display, window);
  }
  XStoreName(display, window, title);
  XGetGeometry(display, window, &winDummy, &x, &y,
               (unsigned int*)&width, (unsigned int *)&height,
               &borderDummy, (unsigned int*)&depth);

  // hide cursor
  //CaptureMouse();
  //Pixmap cursormask; 
  //XGCValues xgc;
  //GC gc;
  //XColor dummycolour;
  //Cursor cursor;
  //cursormask = XCreatePixmap(display, window, 1, 1, 1);
  //xgc.function = GXclear;
  //gc =  XCreateGC(display, cursormask, GCFunction, &xgc);
  //XFillRectangle(display, cursormask, gc, 0, 0, 1, 1);
  //dummycolour.pixel = 0;
  //dummycolour.red = 0;
  //dummycolour.flags = 04;
  //cursor = XCreatePixmapCursor(display, cursormask, cursormask,
  //                             &dummycolour,&dummycolour, 0,0);
  //XFreePixmap(display,cursormask);
  //XFreeGC(display,gc);
  //XDefineCursor(display, window, cursor);
  // grab the pointer
  //XGrabPointer(display, window,
  //             True,
  //             0,
  //             GrabModeAsync, GrabModeAsync,
  //             window,
  //             None,
  //             CurrentTime);

  //XWarpPointer(display, None, window, 0, 0, 0, 0, width / 2, height / 2);
  /*
    XGrabKeyboard(display, window,
    False,
    GrabModeAsync, GrabModeAsync,
    CurrentTime);
  */
}



IRenderer* WindowedApp::CreateRenderer(const char* API)
{
  IRenderer* renderer = new Renderer();
  renderer->Create(API, (int)window, (int)display, (int)screen);
  return renderer;
}


void WindowedApp::ReleaseMouse()
{
  XUndefineCursor(display, window);
  XUngrabPointer(display, CurrentTime);
  mouseCaptured = 0;
}


void WindowedApp::CaptureMouse()
{
  Pixmap cursormask; 
  XGCValues xgc;
  GC gc;
  XColor dummycolour;
  Cursor cursor;
  cursormask = XCreatePixmap(display, window, 1, 1, 1);
  xgc.function = GXclear;
  gc =  XCreateGC(display, cursormask, GCFunction, &xgc);
  XFillRectangle(display, cursormask, gc, 0, 0, 1, 1);
  dummycolour.pixel = 0;
  dummycolour.red = 0;
  dummycolour.flags = 04;
  cursor = XCreatePixmapCursor(display, cursormask, cursormask,
                               &dummycolour,&dummycolour, 0,0);
  XFreePixmap(display,cursormask);
  XFreeGC(display,gc);
  XDefineCursor(display, window, cursor);
  XGrabPointer(display, window,
               True,
               0,
               GrabModeAsync, GrabModeAsync,
               window,
               None,
               CurrentTime);
  XWarpPointer(display, None, window, 0, 0, 0, 0, m_width / 2, m_height / 2);
  mouseCaptured = 1;
}

int WindowedApp::Main(int argc, char** argv)
{
  WindowedApp* theApp = (WindowedApp*)App::theApp;

  printf("WindowedApp, linux\n");

  // precreate
//  if(!theApp->OnPreCreate())
//    return -1;


  // create window
  CreateWindow(m_title, m_xPosition, m_yPosition,
               m_width, m_height, m_fullscreen);

  CaptureMouse();
  // create renderer


  
  // initialize app
  if(theApp->OnInitialize(argc, argv))
  {
    // show window

    XEvent event;

    while(runApp)
    {
      if(XPending(display))
      {
        XNextEvent(display, &event);
        if(!XWindowProc(display, window, event))
          break;
      }
      else
      {
        theApp->OnIdle();
      }
    }
  }

  // terminate
  theApp->OnTerminate();
  XDestroyWindow(display, window);
  delete modes;

  return 0;
}


