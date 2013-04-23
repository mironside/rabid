/**
  @file commandSystem.h
  @author Christopher Olsen
  @date Thu Dec 16 01:37:39 2004


*/
#ifndef __ICOMMAND_SYSTEM__
#define __ICOMMAND_SYSTEM__

typedef void (*cmdFunc_t) (const char* args);

class ICommandSystem
{
public:
  virtual int Initialize() = 0;
  virtual void Release() = 0;

  virtual void AddCommand(const char* name, cmdFunc_t function) = 0;
  virtual int Execute(const char* cmd, const char* args) = 0;
  virtual void ListCommands(const char* args) = 0;
};


#ifdef GAME
extern ICommandSystem* g_commandSystem;
#endif

#endif
