// GLIndexBuffer.cpp
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
#endif


GLIndexBuffer::GLIndexBuffer(unsigned int _format, unsigned int _nIndices) :
  IndexBuffer(_format, _nIndices)
{
  glGenBuffers(1, &glIb);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, glIb);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, size, 0, GL_DYNAMIC_DRAW);
}



GLIndexBuffer::~GLIndexBuffer()
{
  if(locked)
    _Unlock();
  glDeleteBuffers(1, &glIb);
}



void* GLIndexBuffer::_Lock(unsigned int lockType)
{
  void* buf = 0;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, glIb);
  buf = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
  locked = true;

  return buf;
}


void GLIndexBuffer::_Unlock()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, glIb);
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
  locked = false;
}



unsigned int GLIndexBuffer::GetGLFormat()
{
  unsigned int f = 0;
  switch(format)
  {
  case IBF_UINT:
    f = GL_UNSIGNED_INT;
    break;

  default:
    break;
  }

  return f;
}

