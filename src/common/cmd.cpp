//commands
#include "common/cmd.h"
#include <string.h>
#include <stdlib.h>
#include "common/cbuf.h"


void cmdQuit(char* argv)
{
//  commonQuit();
}




void cmdCmdList(char* argv)
{
  cbufListCommands();
}

void cmdCvarList(char* argv)
{
//  cvarList();
}


void cmdError(char* argv)
{
//  sysPrint(argv);
}


void cmdPrint(char* argv)
{
//  sysPrint(argv);
}
