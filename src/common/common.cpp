// common.cpp
#include "common/common.h"
#include <stdarg.h>
#include <stdio.h>
#include "common/console.h"
#include "common/commandSystem.h"
#include "common/cvarSystem.h"

#ifdef DEDICATED
#include "dedicated.h"
extern DedicatedCatharsisEngine* engine;
#endif

#include "common/global.h"

extern Console* g_console;

Common::Common()
{
}



Common::~Common()
{
}

void Common::ExecuteGeneral(const char *name, const char *args)
{
  if(g_commandSystem->Execute(name, args))
  {
    CVar* cvar = g_cvarSystem->Find(name);
    // cvar
    if(cvar)
    {
      if(args)
        cvar->Set(args);
      else
        g_common->Print("%s = %s", name, cvar->GetString());
    }

  }
}

// argv = { cmd, arg1, arg2, ... }
void Common::ExecuteGeneral(int argc, char** argv)
{
  char *name = argv[0];
  if (argc < 1) 
    return;
  char args[512]; args[0] = 0;
  int spaceLeft = 512;
  for (int i = 1; i < argc; i++) {
    strncat(args, argv[i], spaceLeft);
    spaceLeft -= (int)strlen(argv[i]);
    strncat(args, " ", spaceLeft--);
  }
  if (strlen(args) > 0)
    args[strlen(args)-1] = 0;
  ExecuteGeneral(name, args);
}

void Common::Print(const char* fmt, ...)
{
  char text[2048];
  va_list ap;

  va_start(ap, fmt);
  vsprintf(text, fmt, ap);
  va_end(ap);

#ifdef DEDICATED
  engine->Print(text);
#else
  g_console->AddText(text);
#endif
}

