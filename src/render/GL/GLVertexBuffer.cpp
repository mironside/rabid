/**
  @file GLVertexBuffer.cpp
  @author Christopher Olsen
  @date Fri Apr  8 01:29:07 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include "glext.h"


#ifdef WIN32
extern PFNGLBINDBUFFERARBPROC       glBindBuffer;
extern PFNGLGENBUFFERSARBPROC       glGenBuffers;
extern PFNGLBUFFERDATAARBPROC       glBufferData;
extern PFNGLDELETEBUFFERSARBPROC    glDeleteBuffers;
extern PFNGLMAPBUFFERPROC           glMapBuffer;
extern PFNGLUNMAPBUFFERPROC         glUnmapBuffer;
extern PFNGLBUFFERSUBDATAPROC       glBufferSubData;
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2D;
#endif


unsigned int GetGLPrimitiveType(unsigned int type)
{
  float glType = 0;
  switch(type)
  {
  case R_TRIANGLES:
    glType = GL_TRIANGLES;
    break;

  case R_TRIANGLE_STRIP:
    glType = GL_TRIANGLE_STRIP;
    break;

  case R_QUADS:
    glType = GL_QUADS;
    break;

  case R_POINTS:
    glType = GL_POINTS;
    break;

  case R_LINES:
    glType = GL_LINES;
    break;

  case R_LINE_STRIP:
    glType = GL_LINE_STRIP;
    break;

  default:
    break;
  }

  return glType;
}


GLVertexBuffer::GLVertexBuffer(unsigned int vSize, unsigned int nVertices,
                               unsigned int usage) :
  VertexBuffer(vSize, nVertices, usage)
{
  glGenBuffers(1, &glVb);
  glBindBuffer(GL_ARRAY_BUFFER_ARB, glVb);
  glBufferData(GL_ARRAY_BUFFER_ARB, size, 0, GL_DYNAMIC_DRAW);
}



GLVertexBuffer::~GLVertexBuffer()
{
  if(locked)
    _Unlock();
  glDeleteBuffers(1, &glVb);
}



void* GLVertexBuffer::_Lock(unsigned int lockType)
{
  glBindBuffer(GL_ARRAY_BUFFER_ARB, glVb);
  void* buf = glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE);
  int err = glGetError();

  switch(err)
  {
  case GL_OUT_OF_MEMORY:
    break;

  case GL_INVALID_ENUM:
    break;

  case GL_INVALID_OPERATION:
    break;
  }

  if(buf)
    locked = true;

  return buf;
}



void GLVertexBuffer::_Unlock()
{
  glBindBuffer(GL_ARRAY_BUFFER_ARB, glVb);
  glUnmapBuffer(GL_ARRAY_BUFFER_ARB);
  glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
  locked = false;
}



void GLVertexBuffer::Render(unsigned int type, unsigned int nItems,
                            unsigned int offset, IndexBuffer* ib)
{
  GLIndexBuffer* glIb = (GLIndexBuffer*)ib;
  if(glIb)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, glIb->GetGLHandle());
    glEnableClientState(GL_INDEX_ARRAY);
    glIndexPointer(glIb->GetGLFormat(), 0, (unsigned char*)0 + offset);
  }
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    glDisableClientState(GL_INDEX_ARRAY);
  }

  glBindBuffer(GL_ARRAY_BUFFER_ARB, glVb);

  unsigned int size = 0;
  if(format & VF_XYZ)
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, vSize, (unsigned char*)0 + size);
    size += 3*sizeof(float);
  }

  if(format & VF_COLOR)
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, vSize, (unsigned char*)0 + size);
    size += 4*sizeof(float);
  }

  if(format & VF_NORMAL)
  {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, vSize, (unsigned char*)0 + size);
    size += 3*sizeof(float);
  }

  unsigned int nTS = (format & VF_TEXMASK) >> 8;
  for(unsigned int i = 0; i < nTS; i++)
  {
    unsigned int tc = (format >> (i*2+16)) & 0x03;
    if(tc == TEXCOORD1)
      tc = 1;
    else if(tc == TEXCOORD2)
      tc = 2;
    else if(tc == TEXCOORD3)
      tc = 3;
    else if(tc == TEXCOORD4)
      tc = 4;

    glActiveTexture(GL_TEXTURE0+i);
    glClientActiveTexture(GL_TEXTURE0+i);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(tc, GL_FLOAT, vSize, (unsigned char*)0 + size);
    size += tc * sizeof(float);
  }


  if(ib)
    glDrawElements(GetGLPrimitiveType(type), nItems, GL_UNSIGNED_INT, (char*)offset);
  else
    glDrawArrays(GetGLPrimitiveType(type), 0, nItems);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

