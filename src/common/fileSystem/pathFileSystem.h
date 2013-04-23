/**
  @file fileSystem.h
  @author Christopher Olsen
  @date Fri Sep 10 00:48:11 2004
  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __PATH_FILE_SYSTEM__
#define __PATH_FILE_SYSTEM__
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#else
#include <sys/mman.h>
#endif

#include "common/handle.h"
#include "baseFileSystem.h"

/**
   PathFileSystem
   File system that read files from a path.
*/
class PathFileSystem : public BaseFileSystem
{
private:
  char* m_root;

public:
  PathFileSystem(const char* root = ".") { SetRoot(root); }
  virtual ~PathFileSystem();
  
  void SetRoot(const char* path);
  char* GetRoot() { return m_root; }


  bool Exists(const char* filename);
  FileHandle Open(const char* filename);
  void Close(FileHandle file);
};
#endif

