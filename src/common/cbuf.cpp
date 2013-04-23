/**
  @file    cbuf.cpp
  @author  Chris Olsen
  @date    06-10-04
  Timer functions for getting time and, um, stuff.

  Copyright (C) 2004 Christopher A Olsen
 */
#include "cbuf.h"
#include <memory.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "common/cmd.h"
#include <stdarg.h>


#define CBUF_SIZE 8192
char cbufBuffer[8192];
int cbufPos = 0;


typedef struct cmd_s
{
  char name[32];
  void (*func)(char* argv);

  struct cmd_s* llnext;
} cmd_t;


typedef struct execCmd_s
{
  void (*func)(char* argv);
  char argv[128];

  struct execCmd_s* llnext;
} execCmd_t;

static cmd_t* cbufCommands = 0;


void sysPrint(char* fmt, ...)
{
  char text[256];
  va_list ap;
    
  if(fmt == NULL)
    return;
    
  va_start(ap, fmt);
  vsprintf(text, fmt, ap);
  va_end(ap);

  // add text to cbufBuffer
  if(cbufPos + strlen(text) < CBUF_SIZE)
  {
    cbufBuffer[cbufPos++] = '\n';
    strcpy(&cbufBuffer[cbufPos], text);
    cbufPos += (int)strlen(text);
  }
  else
  {
    // wrap the buffer
  }
}



int cbufInitialize()
{
  // override sys print
  memset(cbufBuffer, 0, CBUF_SIZE);
  cbufPos = 0;

  cbufCommands = 0;
  return 0;
}


int cbufExec(char* command, int when)
{
  char cmdstr[256];
  memset(cmdstr, 0, 256);
  strcpy(cmdstr, command);

  int i = 0;
  char* args = 0;
  while(cmdstr[i] != ' ' &&
        cmdstr[i] != 0) 
  {
    i++;
  }

  if(cmdstr[i] != 0)
  {
    cmdstr[i] = 0;
    args = &cmdstr[i+1];
  }

  cmd_t* cmd = 0;
  for(cmd_t* c = cbufCommands; c; c = c->llnext)
  {
    if(strcmp(cmdstr, c->name) == 0)
    {
      cmd = c;
      break;
    }
  }

  if(!cmd)
  {
    return 0;
  }


  switch(when)
  {
  case CBUF_NOW:
    cmd->func(args);
    break;

  case CBUF_ADD:
    break;

  default:
    break;
  }

  return 1;
}


int cbufAddCommand(char* command, void (*func)(char* argv))
{
  cmd_t* cmd = (cmd_t*)malloc(sizeof(cmd_t));
  memset(cmd, 0, sizeof(cmd_t));

  strcpy(cmd->name, command);
  cmd->func = func;
  cmd->llnext = cbufCommands;
  cbufCommands = cmd;

  return 0;
}


char* cbufUpdate()
{
  // exec all waiting commands
  return 0;
}


int cbufDestroy()
{
  cmd_t* cmdTemp = 0;

  for(cmd_t* c = cbufCommands; c; )
  {
    cmdTemp = c->llnext;

    free(c);

    c = cmdTemp;
  }
  cbufCommands = 0;

  return 0;
}


void cbufListCommands()
{
  sysPrint("\nCommands:");
  for(cmd_t* c = cbufCommands; c; c = c->llnext)
  {
    sysPrint("%s", c->name);
  }
}



int cbufListCompletions(char* cmd)
{
  if(cmd[0] == 0)
    return 0;

  int len = (int)strlen(cmd);
  int nCompletions = 0;
  for(cmd_t* c = cbufCommands; c; c = c->llnext)
  {
    if(strncmp(c->name, cmd, len) == 0)
    {
      sysPrint(c->name);
      nCompletions++;
    }
  }

  return nCompletions;
}


char* cbufGetCompletion(char* cmd)
{
  if(cmd[0] == 0)
    return 0;

  int len = (int)strlen(cmd);
  char* completion = 0;
  int nCompletions = 0;
  for(cmd_t* c = cbufCommands; c; c = c->llnext)
  {
    if(strncmp(c->name, cmd, len) == 0)
    {
      nCompletions++;
      if(!completion)
        completion = c->name;
    }
  }

  if(nCompletions > 1)
    completion = 0;

  return completion;
}
