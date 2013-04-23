/**
  @file commandSystem.cpp
  @author Christopher Olsen
  @date Thu Dec 16 01:39:56 2004


*/
#include "common/commandSystem.h"
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include "common/log.h"
#include "common/common.h"
#include "libsn/vector.h"
#include "common/global.h"

void cmdListCommands(const char* args)
{
  LOG(0, "LIST COMMANDS");
  g_commandSystem->ListCommands(args);
}



CommandSystem::CommandSystem()
{
}



CommandSystem::~CommandSystem()
{
}



int CommandSystem::Initialize()
{
  AddCommand("ListCommands", cmdListCommands);
  return 1;
}



void CommandSystem::Release()
{
}



void CommandSystem::AddCommand(const char* name, cmdFunc_t function)
{
  commands[name] = function;
  LOG(0, "COMMAND: '%s'", name);
}



int CommandSystem::Execute(const char* cmd, const char* args)
{
  if(commands.Exists(cmd))
    commands[cmd](args);

  return 0;
}


void CommandSystem::ListCommands(const char* args)
{
  snVector<const char*> keys = commands.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
    g_common->Print("%s", keys[i]);
}



const char* CommandSystem::Complete(const char* cmd)
{
  snVector<const char*> keys = commands.GetKeys();
  snVector<const char*> cmds;

  for(unsigned int i = 0; i < keys.Size(); i++)
  {
    if(strncmp(cmd, keys[i], strlen(cmd)) == 0)
      cmds.PushBack(keys[i]);
  }

  if(cmds.Size() == 1)
  {
    return cmds[0];
  }
  else
  {
    for(unsigned int i = 0; i < cmds.Size(); i++)
      g_common->Print("%s", cmds[i]);
  }

  return 0;
}

