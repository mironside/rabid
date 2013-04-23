/**
  @file log.cpp
  @author Christopher Olsen
  @date Mon Sep  6 18:29:49 2004

  Copyright (C) 2004 Christopher A Olsen

  @todo force log file to have no buffer (writes immediatly to disk,
  in case of crash)
*/
#ifdef LOGGING

#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include "libsn/vector.h"
#include "common/global.h"


FILE* logFile = 0;
snVector<const char*> funcStack;
int newScope = 1;



/**
   StartLog()
   Opens and prepares a new log file.
*/
void StartLog(const char* name)
{
  logFile = fopen(name, "w");
  if(!logFile)
     return;

  setbuf(logFile, 0);

  // print html header
  fputs("<HTML>\n<body>\n<title>Log File</title>\n</body>\n<font face=\"Courier\">\n", logFile);
}



/**
   StopLog()
   Finishes and closes html log.
*/
void StopLog()
{
  if(!logFile)
    return;
  fputs("</font>\n</HTML>\n", logFile);
  fclose(logFile);
  logFile = 0;
}



/**
   Log()
   Writes a formatted argument string to the log file.
*/
void Log(unsigned int options, const char* string, ...)
{
  char text[2048];
  va_list ap;

  if(string == NULL)
    return;
  if(logFile == NULL)
    return;

  va_start(ap, string);
  vsprintf(text, string, ap);
  va_end(ap);


  if(logFile)
  {
    if(newScope)
    {
/*
      for(unsigned int i = 0; i < funcStack.size(); i++)
      {
        for(unsigned int j = 0; j < i; j++)
        {
          fputs("&nbsp;&nbsp;", logFile);
        }
        fputs(funcStack[i], logFile);
        fputs("<br>\n", logFile);
      }
*/
      newScope = 0;
    }

    if(options & LOG_BOLD)
      fputs("<b>", logFile);
    if(options & LOG_ITALIC)
      fputs("<i>", logFile);
    if(options & LOG_RED)
      fputs("<font color=\"#ff0000\">", logFile);
    if(options & LOG_GREEN)
      fputs("<font color=\"#00ff00\">", logFile);
    if(options & LOG_BLUE)
      fputs("<font color=\"#0000ff\">", logFile);


    for(unsigned int i = 0; i < funcStack.Size(); i++)
    {
      fputs("&nbsp;&nbsp;", logFile);
    }

    fputs(text, logFile);

    if(options & LOG_BOLD)
      fputs("</b>", logFile);
    if(options & LOG_ITALIC)
      fputs("</i>", logFile);
    if(options & LOG_RED)
      fputs("</font>", logFile);
    if(options & LOG_GREEN)
      fputs("</font>", logFile);
    if(options & LOG_BLUE)
      fputs("</font>", logFile);

    fputs("<br>\n", logFile);
  }
}





/**
   LogFN::LogFN()
   Pushes the function name onto the function stack.
*/
LogFN::LogFN(unsigned int options, const char* funcName)
{
#ifdef FUNC_TRACE
  // @todo it may be faster to print this to the log directly so the
  // const string isn't parsed by va_arg
  Log(options, funcName);
#endif

  funcStack.PushBack(funcName);
  newScope = 1;
}



/**
   LogFN::~LogFN()
   Pops the last function name off the function stack.
*/
LogFN::~LogFN()
{
  newScope = 1;
  funcStack.PopBack();
}

#endif
