// console.h

#ifndef __CONSOLE__
#define __CONSOLE__

class IRenderer;
struct inputMsg_s;
class Material;
#include "libsn/vector.h"

class Console
{
private:
  int width, height;
  int bufPos;
  char cmd[256];
  int cmdPos;
  Material* text;
  Material* bgShader;
  int lshift;
  int rshift;
  char buffer[8192];
  int cmdIdx;

  int active;
  float conPos;

public:
  Console();
  virtual ~Console();

  int  Active();
  int  Update(float dt);
  void Draw(IRenderer* renderer);
  void AddText(char* text);
  void AddCmdText(char* text);
  void Toggle();
  int OnKeyPress(int key);
  int OnKeyRelease(int key);
  void SetScreenDimensions(int w, int h);
};

#endif
