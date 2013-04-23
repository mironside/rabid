/**
  @file log.h
  @author Christopher Olsen
  @date Mon Sep  6 17:49:28 2004

  Copyright (C) 2004 Christopher A Olsen

  Logs information to an html based log file.  Based on ideas from
  Game Programming Gems 4.

  Usage:
  
  LOG_START(filename)
    opens the given file as a new html log.
    
  LOG_STOP()
    closes the current log file.
    
  LOG(options, string, ...)
    writes a log message with some formatting options which can be
    ORed together: LOG_RED, LOG_GREEN, LOG_BLUE, LOG_BOLD, LOG_ITALIC.
  
  FN(options, string)
    defines a new function scope with formatting options. This should
    be defined at the very beginning of every function to be tracked
    by the logger.

  @todo should be able to open and log to different log files
*/
#ifndef __LOG__
#define __LOG__

#ifdef LOGGING

#define LOG_RED    1
#define LOG_GREEN  2
#define LOG_BLUE   4
#define LOG_BOLD   8
#define LOG_ITALIC 16

#define LOG_RNS LOG_BLUE
#define LOG_NET LOG_BLUE
#define LOG_FUNC  (LOG_ITALIC | LOG_GREEN)


/**
   A utility class for maintaining a function stack
*/
class LogFN
{
private:

public:
  LogFN(unsigned int options, const char* funcName);
  ~LogFN();
};

#define LOG_START(name)  StartLog(name);
#define LOG_STOP()       StopLog();
#define FN(op, name)     LogFN __logScopeFN(op, name);
#define LOG              Log


void StartLog(const char* name);
void StopLog();
void Log(unsigned int options, const char* string, ...);
#else

#define LOG_RED    0
#define LOG_GREEN  0
#define LOG_BLUE   0
#define LOG_BOLD   0
#define LOG_ITALIC 0

#define LOG_RNS    0
#define LOG_NET    0
#define LOG_FUNC   0


#define LOG_START(name)
#define LOG_STOP()
#define FN(op, name)
#define LOG              0 &&

#endif

#endif

