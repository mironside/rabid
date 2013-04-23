// consolse.cpp
#include "common/console.h"
#include "common/profile.h"
#include <memory.h>
#include <string.h>
#include "common/cbuf.h"
#include "public/render.h"
#include "public/cvarSystem.h"
#include "common/commandSystem.h"
#include "common/log.h"
#include "common/common.h"
#include "windowedApp.h"
#include "keys.h"
#include "common/global.h"

CVar contrail("contrail", "1");

int shiftTable[96] = {' ', '!', '\"', '#', '$', '%', '&', '\'',
                      '(', ')', '*', '+', '<', '_', '>', '?',
                      ')', '!', '@', '#', '$', '%', '^', '&',
                      '*', '(', ':', ':', '<', '+', '>', '?',
                      '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                      'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
                      'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                      'X', 'Y', 'Z', '{', '|', '}', '^', '_',
                      '`', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                      'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
                      'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                      'X', 'Y', 'Z', '{', '|', '}', '~', '_'};


Console::Console() :
  bufPos(0), cmdPos(2), text(0), bgShader(0),
  lshift(0), rshift(0), cmdIdx(0), active(0), conPos(0.0f)
{
  memset(buffer, 0, 8192);
  memset(cmd, 0, 256);
  cmd[0] = '>';
  cmd[1] = ' ';
}



Console::~Console()
{
}



int Console::Update(float dt)
{
  if(active)
  {
    if(conPos < ((float)height * 0.5f))
    {
      conPos += (500.0f * dt);
    }

    if(conPos > ((float)height * 0.5f))
    {
      conPos = (float)height * 0.5f;
    }
  }

  if(!active)
  {
    if(conPos > 0)
    {
      conPos -= (500.0f * dt);
    }

    if(conPos < 0)
    {
      conPos = 0;
    }
  }

  return 0;
}



int Console::OnKeyPress(int key)
{
  if(key >= ' ' && key < 127)
  {
    if(lshift || rshift)
      cmd[cmdPos++] = (char)shiftTable[key-32];
    else
      cmd[cmdPos++] = (char)key;
  }
  else if(key == K_BACKSPACE)
  {
    cmdPos--;
    if(cmdPos < 2)
      cmdPos = 2;

    cmd[cmdPos] = 0;
  }
  else if(key == K_ENTER)
  {
    if(cmd[2] == 0)
      return true;

    //exec
    char lcmd[256];
    cmd[cmdPos] = '\0';
    strcpy(lcmd, &cmd[2]);
    char* name = strtok(lcmd, " ");
    char* args;
    if ( (&cmd[2])[strlen(name)] == 0 )
      args = NULL;
    else
      args = lcmd + strlen(name) + 1;
    if(!g_commandSystem->Execute(name, args))
    {
      CVar* cvar = g_cvarSystem->Find(name);
      // cvar
      if(args)
      {
        if(cvar)
          cvar->Set(args);
      }

      if(cvar)
        g_common->Print("%s = %s", name, cvar->GetString());
    }
    else
    {
    }

    memset(cmd, 0, 256);
    cmd[0] = '>';
    cmd[1] = ' ';
    cmdPos = 2;
  }

  else if(key == K_LSHIFT)
  {
    lshift = 1;
  }
  else if(key == K_RSHIFT)
  {
    rshift = 1;
  }
  else if(key == K_TAB)
  {
    const char* txt = 0;
    const char* cCmd  = g_commandSystem->Complete(&cmd[2]);
    const char* cCvar = g_cvarSystem->Complete(&cmd[2]);

    if(cCmd)
      txt = cCmd;
    else if(cCvar)
      txt = cCvar;

    if(txt)
    {
      memset(cmd, 0, 256);
      cmd[0] = '>';
      cmd[1] = ' ';

      strcpy(&cmd[2], txt);
      cmd[strlen(txt)+2] = ' ';
      cmdPos = (int)(strlen(txt)+3);
    }
  }

  // command cycling
  else if(key == K_UP)
  {
    cmdIdx--;
    if(cmdIdx < 0)
      cmdIdx = 0;
    
    memset(cmd, 0, 256);
    cmd[0] = '>';
    cmd[1] = ' ';
  }
  else if(key == K_DOWN)
  {
  }


  return 1;
}


int Console::OnKeyRelease(int key)
{
  if(key == K_LSHIFT)
  {
    lshift = 0;
    return true;
  }
  else if(key == K_RSHIFT)
  {
    rshift = 0;
    return true;
  }
  return false;
}


void Console::Draw(IRenderer* renderer)
{
  if(!text)
    text = renderer->GetMaterial("console_text");
  if(!bgShader)
    bgShader = renderer->GetMaterial("console_bg");

  int maxLines = (int)(conPos / 12);
  int pos = bufPos;
  int posEnd = bufPos;

  int trailPos = (int)(height * 0.11f);

  // Draw console trail
  if(!active && (conPos == 0) && contrail.GetBool())
  {
    renderer->SetOrthoProjection();
    renderer->SetViewMatrix(0);
    renderer->SetColor(1,1,1);
    renderer->BindMaterial(text);
    // draw bottom up
    for(int i = 0; i < 3; i++)
    {
      while(pos > 0 && pos < 8192 &&
            buffer[pos] != '\n')
      {
        pos--;
      }

      // draw line (pos to bufPos)
      renderer->DrawTextLen(5.0f, (height - (trailPos - ((i+1) * 14))+5.0f),
                            16.0f, &buffer[pos+1], (posEnd-pos-1));

      posEnd = pos;
      pos--;

      if(pos < 0)
      {
        break;
      }
    }
    return;
  }


  // Draw Active Console
  renderer->SetOrthoProjection();
  renderer->SetViewMatrix(0);
  renderer->BindMaterial(bgShader);
  renderer->SetColor(1,1,1);
  renderer->DrawRect(0.0, (height - conPos), (float)width, conPos, 0.0f,0.0f, 1.0f,(conPos / (height*0.5f)));

  renderer->BindMaterial(text);
  // draw bottom up
  for(int i = 0; i < maxLines; i++)
  {
    while(pos > 0 && pos < 8192 &&
          buffer[pos] != '\n')
    {
      pos--;
    }

    // draw line (pos to bufPos)
    renderer->DrawTextLen(5.0f, (height - (conPos - ((i+1) * 14))+5.0f),
                          16.0f, &buffer[pos+1], (int)(posEnd-pos-1));

    posEnd = pos;
    pos--;

    if(pos < 0)
    {
      break;
    }
  }

  renderer->DrawTextP(5.0f, (height - conPos+5.0f), 16.0f, cmd);
}



void Console::AddText(char* text)
{
  LOG(LOG_GREEN, "%s\n", text);
  buffer[bufPos++] = '\n';
  if(bufPos + strlen(text) > 8192)
  {
    memset(buffer, 0, 8192);
    bufPos = 0;
  }
  memcpy(&buffer[bufPos], text, strlen(text));
  bufPos += (int)strlen(text);
}



void Console::Toggle()
{
  active ^= 1;
}



int Console::Active()
{
  return active;
}


void Console::SetScreenDimensions(int w, int h)
{
  width = w;
  height = h;
}
