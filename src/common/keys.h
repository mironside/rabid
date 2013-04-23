#ifndef __KEYS__
#define __KEYS__

const int K_ESCAPE =  0;
const int K_LEFT   =  1;
const int K_RIGHT  =  2;
const int K_UP     =  3;
const int K_DOWN   =  4;
const int K_HOME   =  5;
const int K_END    =  6;
const int K_PGUP   =  7;
const int K_PGDN   =  8;
const int K_INS    =  9;
const int K_DEL    = 10;
const int K_F1     = 11;
const int K_F2     = 12;
const int K_F3     = 13;
const int K_F4     = 14;
const int K_F5     = 15;
const int K_F6     = 16;
const int K_F7     = 17;
const int K_F8     = 18;
const int K_F9     = 19;
const int K_F10    = 20;
const int K_F11    = 21;
const int K_F12    = 22;

const int K_LSHIFT    = 23;
const int K_RSHIFT    = 24;
const int K_LCONTROL  = 25;
const int K_RCONTROL  = 26;
const int K_LALT      = 27;
const int K_RALT      = 28;
const int K_CAPS      = 29;
const int K_ENTER     = 30;
const int K_BACKSPACE = 31;
const int K_SPACE     = 32;

// 32 - 127 map to ascii characters

const int K_TAB = 127;
const int K_PRINTSCREEN = 128;
const int K_SCRLK = 129;
const int K_PAUSE = 130;

// keypad
const int K_NUMLK  = 131;
const int KP_SLASH = 132;
const int KP_MUL   = 133;
const int KP_MINUS = 134;
const int KP_HOME  = 135;
const int KP_UP    = 136;
const int KP_PGUP  = 137;
const int KP_LEFT  = 138;
const int KP_5     = 139;
const int KP_RIGHT = 140;
const int KP_END   = 141;
const int KP_DOWN  = 142;
const int KP_PGDN  = 143;
const int KP_INS   = 144;
const int KP_DEL   = 145;
const int KP_PLUS  = 146;
const int KP_ENTER = 147;

// mouse buttons
const int M_LEFT   = 200;
const int M_MIDDLE = 201;
const int M_RIGHT  = 202;
const int M_UP     = 203;
const int M_DOWN   = 204;


const char* KeyName(int key);
int KeyByName(const char* name);

#endif

