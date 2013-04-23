/**
  @file pakFileSystem.cpp
  @author Christopher Olsen
  @date Thu Oct 28 16:03:02 2004

  Copyright (C) 2004 Christopher A Olsen

  @todo PakFileSystem is dependent on PathFileSystem, it would be nice
  if it weren't

  @todo Memory mapping offsets in compressed files is currently not
  handled well.  The file should be decompressed up to the end of the
  requested size.  This is not the best, but compressed files must be
  decompressed from the beginning.
*/
#include <stdio.h>

#include "pakFileSystem.h"
#include <zlib.h>



typedef struct pkFile_s
{
  char name[64];
  unsigned int compression;
  unsigned int compressedSize;
  unsigned int uncompressedSize;
  unsigned int offset;
} pkFile_t;




/**
   memory map package file header
     get id
     get nFiles
     (directory size?)
   close map
   open map on entire directory
     store pkFiles directory pointer
   store data start offset
*/


/**
   OpenPackage
   Opens a pak file and loads it's directory

   pkg - pak filename
*/   
void PakFileSystem::OpenPackage(char* pkg)
{
  if(pkg == 0 || *pkg == 0)
    return;

  strcpy(name, pkg);
  pkgFile = PathFileSystem::Open(pkg);
  if(pkgFile < 0)
    return;
  
  int headerMap = PathFileSystem::OpenMap(pkgFile, 0, 8);
  if(headerMap < 0)
    return;
  
  unsigned int* header = (unsigned int*)PathFileSystem::GetData(headerMap);
  if(header == 0)
    return;
  

  if(header[0] != 0x01010101)
  {
    PathFileSystem::CloseMap(headerMap);
    PathFileSystem::Close(pkgFile);
    return;
  }

  nFiles = header[1];
  PathFileSystem::CloseMap(headerMap);

  int directoryMap = PathFileSystem::OpenMap(pkgFile, 8,
                                             nFiles * sizeof(pkFile_t));

  if(directoryMap < 0)
    return;
  
  pkFiles = (pkFile_t*)PathFileSystem::GetData(directoryMap);
  if(pkFiles == 0)
    return;


  
  printf("\n%s File List\n", name);
  for(int i = 0; i < nFiles; i++)
  {
    printf("  %04d %04d %04d %04d %s\n", pkFiles[i].offset,
                                         pkFiles[i].compression,
                                         pkFiles[i].compressedSize,
                                         pkFiles[i].uncompressedSize,
                                         pkFiles[i].name);
  }
  printf("\n\n");

  
  dataOffset = nFiles*sizeof(pkFile_t) + 8;
}



/**
   Open
   Opens a file and returns a handle.

   filename - the name of the file to be accessed.

   returns handle of the file, NOTE valid handles are >= 0.
*/
FileHandle PakFileSystem::Open(const char* filename)
{
  if(filename == 0 || *filename == 0)
    return -1;

  for(int i = 0; i < nFiles; i++)
  {
    if(strcmp(pkFiles[i].name, filename) == 0)
    {
      fileEntry_t* fileEntry = 0;
      FileHandle handle = fileHandleMgr.Create(&fileEntry);
      fileEntry->pkFileIndex= i;
      fileEntry->size = pkFiles[i].uncompressedSize;
      return handle;
    }
  }

  return -1;
}



/**
   OpenMap
   Opens a memory mapped view of a file.

   file   - file handle to open new view of
   offset - offset from the start of the file to begin the memory map
   size   - the size in bytes of the memory map

   returns a handle to the memory map, NOTE valid handles are >= 0.
*/
int PakFileSystem::OpenMap(int file, int offset, int size)
{
  if(file < 0)
    return -1;

  fileEntry_t* pkgEntry = fileHandleMgr.Dereference(pkgFile);
  printf("pak %d\n", pkgFile);
  if(!pkgEntry)
    return -1;

  fileEntry_t* fileEntry = fileHandleMgr.Dereference(file);
  if(!fileEntry)
    return -1;

  pkFile_t* pkFile = &pkFiles[fileEntry->pkFileIndex];
  if(!pkFile)
    return -1;

  // the +6 is for the zlib header and footer (2 in front 4 in back)
  if(size == 0)
    size = pkFile->compressedSize+6;

  // recalculate the offset from the start of the data section
  offset = dataOffset + pkFile->offset + offset;

  printf("pak size %d\n", PathFileSystem::GetSize(pkgFile));
  printf("offset %d\n", offset);
  printf("file size %d\n", pkFile->compressedSize);
  printf("file end (in package) %d\n", offset + pkFile->compressedSize);

  int hMem = PathFileSystem::OpenMap(pkgFile, offset, pkFile->compressedSize);
  if(hMem < 0)
    return -1;
  memMap_t* memMap = memHandleMgr.Dereference(hMem);




  // decompress view if necessary
  if(pkFile->compression != 0)
  {
    printf("  '%s' is compressed\n", pkFile->name);
    printf("    %d bytes compressed\n", pkFile->compressedSize);
    printf("    %d bytes uncompressed\n", pkFile->uncompressedSize);


    printf(" memMap %d 0x%X\n", hMem, memMap);
    printf(" memMap handle 0x%X\n", memMap->reqView);
    printf(" memMap start 0x%X\n", *(unsigned int*)memMap->view);

    unsigned long destSize = pkFile->uncompressedSize;
    unsigned char* newBuf = new unsigned char[destSize];
    memset(newBuf, 0, destSize);

    printf("0x%X %d 0x%X %d\n", newBuf, destSize, memMap->reqView,
           pkFile->compressedSize);

    // TERRIBLE BUG: Because a zip file does not store a adler-32
    // compress decompresses everything fine then reports an error
    // because the adler-32 is missing.  adler-32 needs to be run on
    // uncompressed data, but that cannot be done because the data is
    // compress outside repack.  so, we will assume successful
    // decompression if destSize == pkFile->uncompressedSize.

    // WARNING: memMap->reqView will not be at the start of a zip
    // stream if there is an offset from the start of the FILE, hence
    // it will fail.
    int error = uncompress(newBuf, &destSize, (unsigned char*)memMap->reqView,
                           pkFile->compressedSize);

    if(destSize != pkFile->uncompressedSize)
    {
      if(error == Z_MEM_ERROR)
        printf("***ERROR: zlib memory error\n");
      else if(error == Z_VERSION_ERROR)
        printf("***ERROR: zlib version error\n");
      else if(error == Z_BUF_ERROR)
        printf("***ERROR: zlib buffer error\n");
      else if(error == Z_DATA_ERROR)
        printf("***ERROR: zlib data error\n");
    }

    // decompress into new buffer (data + offset - data + offset +
    // compressedSize)
    // set decompressed buffer memory map type
    memMap->type = MAP_BUFFERED;
    memMap->view = memMap->reqView = newBuf;
  }

  return hMem;
}

