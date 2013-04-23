/**
  @file baseFileSystem.cpp
  @author Christopher Olsen
  @date Thu Oct 28 18:52:22 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "baseFileSystem.h"
#include "common/global.h"

#ifdef WIN32
#else
#include <sys/mman.h>
#endif
#include "common/log.h"


unsigned int BaseFileSystem::GetSize(FileHandle file)
{
  fileEntry_t* fileEntry = fileHandleMgr.Dereference(file);
  return fileEntry ? fileEntry->size : 0;
}



void* BaseFileSystem::GetData(FileHandle file)
{
  fileEntry_t* fileEntry = fileHandleMgr.Dereference(file);
  return fileEntry ? fileEntry->view : 0;
}
