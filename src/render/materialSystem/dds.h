/**
  @file dds.h
  @author Christopher Olsen
  @date Wed Feb 23 13:44:24 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __DDS__
#define __DDS__

unsigned char* ReadDDS(unsigned char* buffer, unsigned int* width,
                       unsigned int* height, unsigned int* format);

#endif
