/**
  @file fileSystem.cpp
  @author Christopher Olsen
  @date Fri Oct 29 15:09:14 2004

  Copyright (C) 2004 Christopher A Olsen

  @todo come up with a better name for fileHandle_t (file/mem remapping
  handle from fileSystem to file system from which the file/mem was
  loaded).
*/
#include <sys/stat.h>
#include <stdio.h>
#include "fileSystem.h"
#include "baseFileSystem.h"
#include "pathFileSystem.h"
#include "common/log.h"
#include "common/common.h"
#include "common/global.h"

//#include "pakFileSystem.h"
#ifdef WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#ifndef S_ISDIR
#define S_ISTYPE(file, type)   (((file) & _S_IFMT) == (type))
#define S_ISDIR(file)  S_ISTYPE((file), _S_IFDIR)
#define S_ISCHR(file)  S_ISTYPE((file), _S_IFCHR)
#define S_ISBLK(file)  S_ISTYPE((file), _S_IFBLK)
#define S_ISREG(file)  S_ISTYPE((file), _S_IFREG)
#endif

void FixPath(char* path)
{
  if(!path)
    return;

  for(unsigned int i = 0; i < strlen(path); i++)
  {
    if(path[i] == '\\')
      path[i] = '/';
  }
}


FileSystem::FileSystem() :
  fileSystems(0), fileHandleMgr()
{
}



FileSystem::~FileSystem()
{
  for(unsigned int i = 0; i < fileSystems.Size(); i++)
    delete fileSystems[i];
}



/**
   AddPath
   Adds a path to the file source list.

   path - the name of the path to add.
*/
void FileSystem::AddPath(const char* path)
{
  char curPath[512];
  curPath[0] = '\0';
  if(!path || path[0] == '\0')
  {
    getcwd(curPath, 512);
    PathFileSystem* pathFileSystem = new PathFileSystem(curPath);
    fileSystems.PushBack(pathFileSystem);
  }
  else
  {
    struct stat dir;
    if (path[0] != '/')
      getcwd(curPath, 512);
    strcat(curPath, "/");
    strcat(curPath, path);
    if ( stat(path, &dir) == -1 || !S_ISDIR(dir.st_mode) ) {
      printf( "Error adding path %s: not a directory\n", curPath );
      return;
    }
    printf( "FileSystem::AddPath adding %s\n", curPath );
    PathFileSystem* pathFileSystem = new PathFileSystem(curPath);
    fileSystems.PushBack(pathFileSystem);
  }
}



/**
   AddPackage
   Adds a package file (pak) to the file source list.

   pak - the pak name to add.
*/
void FileSystem::AddPackage(char* pak)
{
//  printf("add package %s\n", pak);
//  PakFileSystem* pakFileSystem = new PakFileSystem();
//  pakFileSystem->OpenPackage(pak);

//  fileSystems.push_back(pakFileSystem);
}



/**
   ListSources
   Prints the current list of file sources to the console.
*/
void FileSystem::ListSources()
{
  printf("FileSystem Sources\n");
  for(unsigned int i = 0; i < fileSystems.Size(); i++)
  {
    //    printf(" %s\n", fileSystems[i]->GetName());
  }
}


bool FileSystem::Exists(const char* filename)
{
  for(unsigned int i = 0; i < fileSystems.Size(); i++)
  {
    if(fileSystems[i]->Exists(filename))
       return true;
  }
  return false;
}



/**
   Open
   Opens a file from the file source list, opens from the first file
   source that was found to contain it.

   filename - the file to open.

   returns - handle to the file, -1 on error.
*/
FileHandle FileSystem::Open(const char* filename)
{
  if(filename == 0 || filename[0] == 0)
    return FileHandle();

  for(unsigned int i = 0; i < fileSystems.Size(); i++)
  {
    if(!fileSystems[i]->Exists(filename))
      continue;

    int file = fileSystems[i]->Open(filename);
    if(file >= 0)
    {
      fileHandle_t* h;
      FileHandle fileHandle(fileHandleMgr.Create(&h));
      h->handle = file;
      h->fs     = fileSystems[i];

      //      printf("Found '%s' in %s\n", filename, fileSystems[i]->GetName());
      return fileHandle;
    }
  }

  return FileHandle();
}



/**
   Close
   Closes a file and destroys it's handle.

   file - file handle to close
*/
void FileSystem::Close(FileHandle file)
{
  if(file < 0)
    return;

  fileHandle_t* h = fileHandleMgr.Dereference(file);
  h->fs->Close(h->handle);
  fileHandleMgr.Destroy(file);
}



unsigned int FileSystem::GetSize(FileHandle file)
{
  fileHandle_t* h = fileHandleMgr.Dereference(file);
  return (h != 0) ? h->fs->GetSize(h->handle) : 0;
}


void* FileSystem::GetData(FileHandle file)
{
  fileHandle_t* h = fileHandleMgr.Dereference(file);
  return (h != 0) ? h->fs->GetData(h->handle) : 0;
}



const char* FileSystem::GetRelativePath(const char* name)
{
  for(unsigned int i = 0; i < fileSystems.Size(); i++)
  {
    char* root = fileSystems[i]->GetRoot();
    if(strncmp(root, name, strlen(root)) == 0)
      return &name[strlen(root)]+1;
  }

  return 0;
}


