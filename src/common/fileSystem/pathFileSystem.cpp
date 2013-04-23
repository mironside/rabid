/**
  @file fileSystem.cpp
  @author Christopher Olsen
  @date Fri Sep 10 00:52:55 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include <stdio.h>
#include <string.h>


#include "common/handle.h"
#include "pathFileSystem.h"
#include "common/log.h"
#include "common/fileSystem/fileSystem.h"

#ifdef WIN32
#define getcwd(s, l)  GetCurrentDirectory(l, s)
#endif

PathFileSystem::~PathFileSystem()
{
  delete m_root;
}


/**
   SetPath
   Sets the search path for the file system.

   path - the new path name.
*/
void PathFileSystem::SetRoot(const char* root)
{
  LOG(0, "Adding Path '%s'", root);
  m_root = new char[strlen(root)+1];
  memcpy(m_root, root, strlen(root)+1);
  FixPath(m_root);
}


bool PathFileSystem::Exists(const char* filename)
{
  char pathName[512];
  sprintf(pathName, "%s/%s", m_root, filename);

  bool exists = false;
  FILE* file = fopen(pathName, "rb");
  if(file)
  {
    exists = true;
    fclose(file);
  }

  return exists;
}



/**
   Open
   Opens a file.

   filename - name of the file to open.

   returns - handle to the file, -1 on error.
*/
FileHandle PathFileSystem::Open(const char* filename)
{
  fileEntry_t* fileEntry = 0;
  FileHandle handle = fileHandleMgr.Create(&fileEntry);

  char pathName[512];
  sprintf(pathName, "%s/%s", m_root, filename);


#ifdef WIN32
  unsigned int access   = GENERIC_READ;
  unsigned int sharing  = FILE_SHARE_READ | FILE_SHARE_WRITE;
  unsigned int creation = OPEN_EXISTING;
  unsigned int flags    = FILE_ATTRIBUTE_NORMAL;

  fileEntry->hFile = CreateFile(pathName, access, sharing, 0, creation,
                                flags, 0);

  if(fileEntry->hFile == INVALID_HANDLE_VALUE)
    goto error;

  fileEntry->hMap  = CreateFileMapping(fileEntry->hFile, 0, PAGE_READONLY,
                                       0, 0, 0);

  if(fileEntry->hMap == INVALID_HANDLE_VALUE)
    goto error;

  fileEntry->size = GetFileSize(fileEntry->hFile, 0);
  if(fileEntry->size <= 0)
    goto error;

  fileEntry->view = (unsigned char*)MapViewOfFile(fileEntry->hMap,
                                                  FILE_MAP_READ, 0, 0, fileEntry->size);



#else
  LOG(0, "Open file '%s'", pathName);
  fileEntry->hFile = open(pathName, O_RDONLY);

  if(fileEntry->hFile < 0)
    goto error;

  struct stat s;
  fstat(fileEntry->hFile, &s);
  fileEntry->size = s.st_size;

  if(fileEntry->size <= 0)
    goto error;

  fileEntry->view  = (unsigned char*)mmap(0, fileEntry->size, PROT_READ,
                                          MAP_PRIVATE | MAP_NORESERVE,
                                          fileEntry->hFile, 0);

  if((int)fileEntry->view == (int)0xFFFFFFFF)
  {
    printf("error! returning -1 from PathFileSystem::OpenMap\n");
    return -1;
  }
#endif

  return handle;




error:
  LOG(0, "failure");
  fileHandleMgr.Destroy(handle);

  return -1;
}



void PathFileSystem::Close(FileHandle file)
{
  fileEntry_t* fileEntry = fileHandleMgr.Dereference(file);
  if(!fileEntry)
    return;

#ifdef WIN32
  UnmapViewOfFile(fileEntry->view);
  CloseHandle(fileEntry->hMap);
  CloseHandle(fileEntry->hFile);
#else
  munmap(fileEntry->view, fileEntry->size);
#endif

  fileHandleMgr.Destroy(file);
}
