// common.h
#ifndef __ICOMMON__
#define __ICOMMON__


/// These color definitions can be used inside a string when calling
/// g_common->Print.  To use them, just close the string, insert the
/// color name, and open the string again.  Using these also gets rid
/// of the problem where letters in the string are interpreted as part
/// of the color.
#define C_BLACK   "\x001"
#define C_BLUE    "\x002"
#define C_GREEN   "\x003"
#define C_CYAN    "\x004"
#define C_RED     "\x005"
#define C_MAGENTA "\x006"
#define C_YELLOW  "\x007"
#define C_WHITE   "\x008"


class ICommon
{
public:
  virtual void Print(const char *fmt, ...) = 0;

  // execute command / assign cvar
  virtual void ExecuteGeneral(int argc, char** argv) = 0;
  virtual void ExecuteGeneral(const char *cmd, const char *args) = 0;
};

#ifdef GAME
extern ICommon* g_common;
#endif

#endif
