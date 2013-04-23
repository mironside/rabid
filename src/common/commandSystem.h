/**
  @file commandSystem.h
  @author Christopher Olsen
  @date Thu Dec 16 01:37:39 2004


*/
#ifndef __COMMAND_SYSTEM__
#define __COMMAND_SYSTEM__

#include "public/commandSystem.h"
#include "libsn/hashMap.h"

typedef void (*cmdFunc_t) (const char* args);

class CommandSystem : public ICommandSystem
{
protected:
  snSHashMap<cmdFunc_t> commands;

public:
  CommandSystem();
  virtual ~CommandSystem();

  int Initialize();
  void Release();

  void AddCommand(const char* name, cmdFunc_t function);
  int Execute(const char* cmd, const char* args);
  void ListCommands(const char* args);
  const char* Complete(const char* cmd);
};

extern CommandSystem* g_commandSystem;

#endif
