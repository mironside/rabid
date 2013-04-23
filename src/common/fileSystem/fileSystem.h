/**
  @file fileSystem.h
  @author Christopher Olsen
  @date Fri Oct 29 15:05:04 2004
  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __FILE_SYSTEM__
#define __FILE_SYSTEM__

#include "common/handle.h"
#include "public/fileSystem.h"
class BaseFileSystem;

/// mapping from FileSystem handle to a handle for the given
/// filesystem the file is in.
typedef struct fileHandle_s
{
  FileHandle handle;
  BaseFileSystem* fs;
} fileHandle_t;

void FixPath(char* path);

/**
   FileSystem
   A file interface that can access files via the path, or package
   files (compressed or uncompressed).
*/
class FileSystem : public IFileSystem
{
protected:
  snVector<BaseFileSystem*> fileSystems;
  HandleMgr<fileHandle_t> fileHandleMgr;

public:
  FileSystem();
  virtual ~FileSystem();

  void AddPath(const char* path = 0);
  void AddPackage(char* pak);
  void ListSources();
  bool Exists(const char* filename);

  // usage
  FileHandle Open(const char* filename);
  void Close(FileHandle file);
  unsigned int GetSize(FileHandle file);

  void* GetData(FileHandle file);
  int Read(void* dst, void* src, unsigned int bytes);

  const char* GetRelativePath(const char* name);
};
#endif

