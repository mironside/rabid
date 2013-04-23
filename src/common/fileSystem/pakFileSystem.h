/**
  @file pakFileSystem.h
  @author Christopher Olsen
  @date Thu Oct 28 16:03:14 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __PAK_FILE_SYSTEM__
#define __PAK_FILE_SYSTEM__

#include "pathFileSystem.h"



struct pkFile_s;


/**
   PakFileSystem
   File system that searches and reads from package files which may or
   may not be compressed.
*/
class PakFileSystem : public PathFileSystem
{
protected:
  struct pkFile_s* pkFiles;
  int nFiles;
  int pkgFile;
  int dataOffset;


public:
  PakFileSystem() {};
  virtual ~PakFileSystem() {};

  /// Opens a package file to use for this file system.
  void OpenPackage(char* pkg);

  /// Opens a file in the package, returns it's handle.
  FileHandle Open(const char* filename);

  /// Opens a memory mapped view of the file, returns it's handle.
  virtual int OpenMap(int hMap, int offset, int size);
};

#endif

