/**
  @file baseFileSystem.h
  @author Christopher Olsen
  @date Thu Oct 28 17:56:03 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __BASE_FILE_SYSTEM__
#define __BASE_FILE_SYSTEM__

#include "common/handle.h"
#include "public/fileSystem.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

enum mapType_e {MAP_NORMAL, MAP_BUFFERED};



typedef struct fileEntry_s
{
#ifdef WIN32
  HANDLE hFile;
  HANDLE hMap;
#else
  int hFile;
#endif
  void* view;
  int size;
} fileEntry_t;


typedef struct memEntry_s
{
  void* data;
  int size;
  FileHandle file;
} memEntry_t;



/**
   BaseFileSystem
   Common interface for file systems used by the FileSystem class.
*/
class BaseFileSystem
{
protected:
  HandleMgr<fileEntry_t> fileHandleMgr;

public:
  BaseFileSystem() {};
  virtual ~BaseFileSystem() {};

  virtual bool Exists(const char* filename) = 0;
  virtual FileHandle Open(const char* filename) = 0;
  virtual void Close(FileHandle file) = 0;
  unsigned int GetSize(FileHandle file);  virtual char* GetRoot() { return 0; };
  void* GetData(FileHandle file);
  int Read(void* dst, void* src, unsigned int size);
};

#endif

