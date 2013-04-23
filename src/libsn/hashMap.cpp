/**
  @file hashMap.cpp
  @author Christopher Olsen
  @date Tue Mar  8 01:53:12 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include <string.h>
#include "common/global.h"

/**
  H A S H   F U N C T I O N S
*/
unsigned int defhash(int i)
{ return (unsigned int)(i); }

unsigned int defhash(const char* str) {
  unsigned int hash = 0;
  int c;

  while((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

unsigned int defhash(char* str)
{ return defhash((const char*)str); }



/**
  C O M P A R E   F U N C T I O N S
*/
int defcmp(const char* str1, const char* str2)
{ return strcmp(str1, str2); }

int defcmp(char* str1, char* str2)
{ return defcmp((const char*)str1, (const char*)str2); }

int defcmp(int i1, int i2)
{ return i2 - i1; }

