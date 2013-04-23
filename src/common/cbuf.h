/**
  @file    cbuf.h
  @author  Chris Olsen
  @date    06-10-04
  Timer functions for getting time and, um, stuff.

  Copyright (C) 2004 Christopher A Olsen
 */
#ifndef __CBUF__
#define __CBUF__

typedef enum {CBUF_NOW, CBUF_ADD};

int cbufInitialize();
int cbufExec(char* command, int when = CBUF_ADD);
int cbufAddCommand(char* command, void (*func)(char* argv));
char* cbufUpdate();
int cbufDestroy();
void cbufListCommands();
int cbufListCompletions(char* cmd);
char* cbufGetCompletion(char* cmd);

extern char cbufBuffer[8192];
extern int cbufPos;

#endif

