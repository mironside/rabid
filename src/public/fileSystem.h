/**
  @file fileSystem.h
  @author Christopher Olsen
  @date Fri Oct 29 15:05:04 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __IFILE_SYSTEM__
#define __IFILE_SYSTEM__

// handles for file and memmaps
class FileHandle
{
public:
  FileHandle(int h = -1) : m_handle(h) {}
  operator int () { return m_handle; }
    

private:
  int m_handle;
};


class MemHandle
{
public:
  MemHandle(int h = -1) : m_handle(h) {}
  operator int () { return m_handle; }

private:
  int m_handle;
};



/**
   FileSystem
   A file interface that can access files via the path, or package
   files (compressed or uncompressed).
*/
class IFileSystem
{
public:
  virtual ~IFileSystem() {}
  virtual void AddPath(const char* path = 0) = 0;
  virtual void AddPackage(char* pak) = 0;
  virtual void ListSources() = 0;
  virtual bool Exists(const char* filename) = 0;
  
  virtual FileHandle Open(const char* filename) = 0;
  virtual void Close(FileHandle file) = 0;
  virtual unsigned int GetSize(FileHandle file) = 0;

  virtual void* GetData(FileHandle hMem) = 0;
  virtual const char* GetRelativePath(const char* name) = 0;
};


extern IFileSystem* g_fileSystem;

#endif

