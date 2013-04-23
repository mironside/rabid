/**
  @file fileSystem.h
  @author Christopher Olsen
  @date Fri Oct 29 15:05:04 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __FILE_SYSTEM__
#define __FILE_SYSTEM__

#include "public/fileSystem.h"

#include "common/handle.h"
class BaseFileSystem;


/// mapping from FileSystem handle to a handle for the given
/// filesystem the file is found in.
typedef struct fileHandle_s
{
  int handle;
  BaseFileSystem* fs;
} fileHandle_t;




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
  HandleMgr<fileHandle_t> memHandleMgr;



public:
  FileSystem();
  virtual ~FileSystem();

  
  /// Adds a path to the file source list
  void AddPath(char* path);

  /// Adds a package to the file source list
  void AddPackage(char* pak);

  /// Prints the file source list
  void ListSources();

  bool Exists(const char* filename);


  /// Opens a file and returns it's handle
  int Open(char* filename);

  /// Closes a file, destroys it's handle
  void Close(int file);

  /// Opens a memory mapped view of a file, returns it's handle
  int OpenMap(int file, int offset, int size);

  /// Closes a memory mapped view of a file
  void CloseMap(int hMem);

  /// Returns a pointer to memory mapped data of the given handle
  void* GetData(int hMem);

  /// Returns the total file size (in bytes) of the file
  unsigned int GetSize(int file);
};

extern FileSystem* g_fileSystem;

#endif

