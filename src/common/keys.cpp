#include "common/keys.h"
#include <string.h>
#include "common/global.h"

typedef struct key_s
{
  int key;
  const char* name;
} key_t;

char charname[2] = {0, 0};

key_t keyNames[] =
{
  // keys
  {K_ESCAPE, "escape"},
  {K_LEFT,   "left"},
  {K_RIGHT,  "right"},
  {K_UP,     "up"},
  {K_DOWN,   "down"},
  {K_HOME,   "home"},
  {K_END,    "end"},
  {K_PGUP,   "pagedown"},
  {K_PGDN,   "pageup"},
  {K_INS,    "insert"},
  {K_DEL,    "delete"},
  {K_F1,     "f1"},
  {K_F2,     "f2"},
  {K_F3,     "f3"},
  {K_F4,     "f4"},
  {K_F5,     "f5"},
  {K_F6,     "f6"},
  {K_F7,     "f7"},
  {K_F8,     "f8"},
  {K_F9,     "f9"},
  {K_F10,    "f10"},
  {K_F11,    "f11"},
  {K_F12,    "f12"},

  {K_LSHIFT,    "lshift"},
  {K_RSHIFT,    "rshift"},
  {K_LCONTROL,  "lcontrol"},
  {K_RCONTROL,  "rcontrol"},
  {K_LALT,      "lalt"},
  {K_RALT,      "ralt"},
  {K_CAPS,      "caps"},
  {K_ENTER,     "enter"},
  {K_BACKSPACE, "backspace"},
  {K_SPACE,     "space"},

  // 32 - 127 map to ascii characters

  {K_TAB,         "tab"},
  {K_PRINTSCREEN, "printscreen"},
  {K_SCRLK,       "scrolllock"},
  {K_PAUSE,       "pause"},

  // keypad
  {K_NUMLK,       "numlock"},
  {KP_SLASH,      "kp_/"},
  {KP_MUL,        "kp_*"},
  {KP_MINUS,      "kp_-"},
  {KP_HOME,       "kp_home"},
  {KP_UP,         "kp_up"},
  {KP_PGUP,       "kp_pageup"},
  {KP_LEFT,       "kp_left"},
  {KP_5,          "kp_5"},
  {KP_RIGHT,      "kp_right"},
  {KP_END,        "kp_end"},
  {KP_DOWN,       "kp_down"},
  {KP_PGDN,       "kp_pagedown"},
  {KP_INS,        "kp_insert"},
  {KP_DEL,        "kp_delete"},
  {KP_PLUS,       "kp_plus"},
  {KP_ENTER,      "kp_enter"},

  // mouse buttons
  {M_LEFT,   "m_left"},
  {M_MIDDLE, "m_middle"},
  {M_RIGHT,  "m_right"},
  {M_UP,     "m_up"},
  {M_DOWN,   "m_down"},

  {-1, 0}
};



int KeyByName(const char* name)
{
  for(int i = 0; keyNames[i].key >= 0; i++)
  {
    if(strcmp(name, keyNames[i].name) == 0)
      return keyNames[i].key;
  }

  return -1;
}


const char* KeyName(int key)
{
  if(key > ' ' && key < 127)
  {
    charname[0] = (char)key;
    charname[1] = 0;
    return charname;
  }
  
  for(int i = 0; keyNames[i].key >= 0; i++)
  {
    if(key == keyNames[i].key)
      return keyNames[i].name;
  }

  return 0;
}
