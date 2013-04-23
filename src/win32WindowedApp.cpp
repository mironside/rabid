/**
  @file linuxWindowedApp.cpp
  @author Christopher Olsen
  @date Sun Jan  9 19:38:26 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "windowedApp.h"
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <windows.h>
#define WM_MOUSEWHEEL		0x020A
#include "render/renderer.h"
#include "common/keys.h"
#include <direct.h>
#include "public/fileSystem.h"
#include "public/common.h"
#include "public/render.h"
#include "common/global.h"

HANDLE hThread = 0;

typedef struct keymap_s
{
  int osKey;
  int appKey;
} keymap_t;


// X keymapping
keymap_t keyMap[] =
{
  {VK_ESCAPE,    K_ESCAPE},
  {VK_LEFT,      K_LEFT},
  {VK_RIGHT,     K_RIGHT},
  {VK_UP,        K_UP},
  {VK_DOWN,      K_DOWN},
  {VK_HOME,      K_HOME},
  {VK_END,       K_END},
  {VK_PRIOR,     K_PGUP},
  {VK_NEXT,      K_PGDN},
  {VK_INSERT,    K_INS},
  {VK_DELETE,    K_DEL},
  {VK_F1,        K_F1},
  {VK_F2,        K_F2},
  {VK_F3,        K_F3},
  {VK_F4,        K_F4},
  {VK_F5,        K_F5},
  {VK_F6,        K_F6},
  {VK_F7,        K_F7},
  {VK_F8,        K_F8},
  {VK_F9,        K_F9},
  {VK_F10,       K_F10},
  {VK_F11,       K_F11},
  {VK_F12,       K_F12},

  {VK_SHIFT,     K_LSHIFT},
  {VK_SHIFT,     K_RSHIFT},
  {VK_CONTROL,   K_LCONTROL},
  {VK_CONTROL,   K_RCONTROL},
  {VK_MENU,      K_LALT},
  {VK_MENU,      K_RALT},
  {VK_CAPITAL,   K_CAPS},
  {VK_RETURN,    K_ENTER},
  {VK_BACK,      K_BACKSPACE},
  {VK_SPACE,     K_SPACE},


  {VK_TAB,       K_TAB},
  {VK_SNAPSHOT,  K_PRINTSCREEN},
  {VK_SCROLL,    K_SCRLK},
  {VK_PAUSE,     K_PAUSE},

  {VK_DIVIDE,    KP_SLASH},
  {VK_MULTIPLY,  KP_MUL},
  {VK_SUBTRACT,  KP_MINUS},
  {VK_NUMPAD7,   KP_HOME},
  {VK_NUMPAD8,   KP_UP},
  {VK_NUMPAD9,   KP_PGUP},
  {VK_NUMPAD4,   KP_LEFT},
  {12,           KP_5},
  {VK_NUMPAD5,   KP_5},
  {VK_NUMPAD6,   KP_RIGHT},
  {VK_NUMPAD1,   KP_END},
  {VK_NUMPAD2,   KP_DOWN},
  {VK_NUMPAD3,   KP_PGDN},
  {VK_NUMPAD0,   KP_INS},
  {VK_DECIMAL,   KP_DEL},
  {VK_ADD,       KP_PLUS},
  {VK_NUMLOCK,   K_NUMLK},
  {0,            KP_ENTER},
  {186, ';'},
  {187, '='},
  {188, ','},
  {189, '-'},
  {190, '.'},
  {191, '/'},
  {192, '`'},
  {222, '\''},
  {219, '['},
  {220, '\\'},
  {221, ']'},

  {-1, -1}
};
extern int main(int argc, char** argv);

HINSTANCE hInstance;
HWND hWnd;
HANDLE hDir;



int os_to_app(int wParam, int lParam)
{
  if(wParam >= 'A' && wParam <= 'Z')
    return wParam - 'A' + 'a';
  if(wParam >= '0' && wParam <= '9')
    return wParam;

  // search mapping
  for(int i = 0; keyMap[i].osKey >= 0; i++)
  {
    if(keyMap[i].osKey == wParam)
    {
      if(lParam & 0x01000000)
      {
        if(keyMap[i].appKey == K_LCONTROL)
          return K_RCONTROL;
        if(keyMap[i].appKey == K_LSHIFT)
          return K_RSHIFT;
        if(keyMap[i].appKey == K_LALT)
          return K_RALT;
      }
      return keyMap[i].appKey;
    }
  }

  return -1;
}


void WindowedApp::RequestTermination()
{
  CloseHandle(hThread);
  OnTerminate();
  PostQuitMessage(0);
}


LRESULT CALLBACK WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  WindowedApp* theApp = (WindowedApp*)App::theApp;

  int xPos = 0;
  int yPos = 0;
  int newWidth;
  int newHeight;
  switch(uMsg)
  {
  case WM_CLOSE:
    //PostQuitMessage(0);
    theApp->RequestTermination();
    break;

  case WM_KEYDOWN:
    theApp->OnKeyPress(os_to_app((int)wParam, (int)lParam));
    break;

  case WM_KEYUP:
    theApp->OnKeyRelease(os_to_app((int)wParam, (int)lParam));
    break;

  case WM_SYSKEYDOWN:
    theApp->OnKeyPress(os_to_app((int)wParam, (int)lParam));
    break;

  case WM_SYSKEYUP:
    theApp->OnKeyRelease(os_to_app((int)wParam, (int)lParam));
    break;

  case WM_LBUTTONDOWN:
    theApp->OnKeyPress(M_LEFT);
    break;

  case WM_LBUTTONUP:
    theApp->OnKeyRelease(M_LEFT);
    break;

  case WM_RBUTTONDOWN:
    theApp->OnKeyPress(M_RIGHT);
    break;

  case WM_RBUTTONUP:
    theApp->OnKeyRelease(M_RIGHT);
    break;

  case WM_MBUTTONDOWN:
    theApp->OnKeyPress(M_MIDDLE);
    break;

  case WM_MBUTTONUP:
    theApp->OnKeyRelease(M_MIDDLE);
    break;

  case WM_MOUSEWHEEL:
    {
      short move = (short)(((int)wParam >> 16) & 0xFFFF);
      int key = M_UP;
      if(move < 0)
        key = M_DOWN;
      theApp->OnKeyPress(key);
      theApp->OnKeyRelease(key);
    }
    break;

  case WM_SIZE:
    newWidth = (int)(short) LOWORD(lParam);  
    newHeight = (int)(short) HIWORD(lParam);
    theApp->Resize(newWidth, newHeight);
    break;

  case WM_MOVE:
    xPos = (int)(short) LOWORD(lParam);   // horizontal position 
    yPos = (int)(short) HIWORD(lParam);   // vertical position 
    theApp->SetXPosition(xPos);
    theApp->SetYPosition(yPos);
    break;

  default:
    return DefWindowProc(wnd, uMsg, wParam, lParam);
  }

	return 0;
}

void CreateWin32Window(const char* title, int& x, int& y,
                       int& width, int& height, int& fullscreen)
{
	WNDCLASS wc;						// Windows Class Structure
	DWORD    dwExStyle;				// Window Extended Style
	DWORD    dwStyle;				// Window Style
	RECT     WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values

	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance        = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc   = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra    = 0;									// No Extra Window Data
	wc.cbWndExtra    = 0;									// No Extra Window Data
	wc.hInstance     = hInstance;							// Set The Instance
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName  = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if(!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth  = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = 32;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return;									// Return FALSE
			}
		}
	}

	if(fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if(!(hWnd=CreateWindowEx(dwExStyle,							// Extended Style For The Window
								            "OpenGL",							// Class Name
								            title,								// Window Title
								            dwStyle |							// Defined Window Style
								            WS_CLIPSIBLINGS |					// Required Window Style
								            WS_CLIPCHILDREN,					// Required Window Style
								            0, 0,								// Window Position
								            WindowRect.right-WindowRect.left,	// Calculate Window Width
								            WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								            NULL,								// No Parent Window
								            NULL,								// No Menu
								            hInstance,							// Instance
								            NULL)))								// Dont Pass Anything To WM_CREATE
	{
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ShowCursor(0);
}

void WindowedApp::CaptureMouse()
{
  WindowedApp* theApp = (WindowedApp*)App::theApp;

  if(m_fullscreen || m_capturemouse)
    return;

  // Make sure ShowCursor returns 1
  do{}while(ShowCursor(0) >= 0);
  SetCursorPos((theApp->GetWidth() >> 1) + theApp->GetXPosition(), (theApp->GetHeight() >> 1) + theApp->GetYPosition());
  m_capturemouse = 1;
}

void WindowedApp::ReleaseMouse()
{
  if(m_fullscreen || !m_capturemouse)
    return;

  // Make sure ShowCursor returns 1
  do{}while(ShowCursor(1) <= 0);
  m_capturemouse = 0;
}

IRenderer* WindowedApp::CreateRenderer(const char* API)
{
  IRenderer* renderer = new Renderer();
  renderer->Create(API, (int)hWnd, (int)hInstance);

  return renderer;
}

typedef struct _DIRECTORY_INFO {
HANDLE      hDir;
TCHAR       lpszDirName[256];
CHAR        lpBuffer[1024];
DWORD       dwBufLength;
OVERLAPPED  Overlapped;
}DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

DIRECTORY_INFO  DirInfo[4];        // Buffer for all of the directories


int WindowedApp::Main(int argc, char** argv)
{
/*
  // startup memory leak detection
  #ifndef NDEBUG
  int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
  flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
  _CrtSetDbgFlag(flag); // Set flag to the new value
  #endif 
*/

  WindowedApp* theApp = (WindowedApp*)App::theApp;

  printf("WindowedApp, linux\n");

  // precreate
//  if(!theApp->OnPreCreate())
//    return -1;


  // create window
  CreateWin32Window(m_title, m_xPosition, m_yPosition,
                    m_width, m_height, m_fullscreen);

  // Capture the mouse
  CaptureMouse();


  // create renderer


  // initialize app
  if(theApp->OnInitialize(argc,argv))
  {
    // Create a thread to sit on the directory changes

    MSG msg;
    POINT p;
    int dx = 0;
    int dy = 0;
    while(1)
    {
      if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
      {
        if(msg.message == WM_QUIT)
          break;

        TranslateMessage(&msg);				// Translate The Message
        DispatchMessage(&msg);				// Dispatch The Message
      }
      else
      {
        if(m_capturemouse)
        {
          // get mouse motion since last update
          GetCursorPos(&p);
          dx = p.x - ((theApp->GetWidth() >> 1) + theApp->GetXPosition());
          dy = p.y - ((theApp->GetHeight() >> 1) + theApp->GetYPosition());
          if(dx != 0 || dy != 0)
          {
            theApp->OnMouseMove(dx, dy);
            SetCursorPos((theApp->GetWidth() >> 1) + theApp->GetXPosition(), (theApp->GetHeight() >> 1) + theApp->GetYPosition());
          }
        }

        // OnIdle processing
        theApp->OnIdle();
      }
    }
  }

  // terminate
  //theApp->OnTerminate();
  //theApp->RequestTermination();

  return 0;
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  char* args[128];
  int toks = 0;
  char* tok = 0;
  if((tok = strtok(lpCmdLine, " ")) != 0)
  {
    do
    {
      toks++;
      args[toks] = tok;
    } while((tok = strtok(0, " ")) != 0);
  }

  /// @todo pass argc, argv in here
  return main(toks+1, args);
}
