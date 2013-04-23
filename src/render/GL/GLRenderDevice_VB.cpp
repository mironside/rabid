/**
  @file GLRenderDevice_VB.cpp
  @author Christopher Olsen
  @date Fri Sep 17 00:17:23 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLRenderDevice.h"
#include "render/renderer_defs.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifndef WIN32
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include <memory.h>
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




unsigned int GLRenderDevice::CreateVB(unsigned int size, unsigned int storageFlags)
{
  unsigned int buf = 0;

  glGenBuffers(1, &buf);
  glBindBuffer(GL_ARRAY_BUFFER_ARB, buf);
  glBufferData(GL_ARRAY_BUFFER_ARB, size, 0, GL_DYNAMIC_DRAW);

  return buf;
}



unsigned int GLRenderDevice::CreateIB(unsigned int size, unsigned int storageFlags)
{
  unsigned int buf = 0;

  glGenBuffers(1, &buf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, size, 0, GL_DYNAMIC_DRAW);

  return buf;
}




// gets a pointer for writing in a vertex buffer.
// takes VB id, and a pointer to set data to.
// returns number of vertices in the VB, 0 on fail.
void GLRenderDevice::LockVB(unsigned int vb, void** buf)
{
  glBindBuffer(GL_ARRAY_BUFFER_ARB, vb);
  *buf = glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
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
}



void GLRenderDevice::LockIB(unsigned int ib, void** buf)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, ib);
  *buf = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
}



void GLRenderDevice::UnlockVB(unsigned int vb)
{
  glBindBuffer(GL_ARRAY_BUFFER_ARB, vb);
  glUnmapBuffer(GL_ARRAY_BUFFER_ARB);
  glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
}



void GLRenderDevice::UnlockIB(unsigned int ib)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, ib);
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}



void GLRenderDevice::DrawVB(unsigned int vb, unsigned int vbOffset,
                            unsigned int ib, unsigned int ibOffset,
                            unsigned int format, unsigned int mode, int nElements)
{
  glBindBuffer(GL_ARRAY_BUFFER_ARB, vb);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, ib);

  /// @todo this could cause problems if one were to send in an incorrect format
  unsigned int vertexSize = 0;
  if(format & R_VB_XYZ)
  {
    vertexSize += 3 * sizeof(float);
  }
  if(format & R_VB_TC0)
  {
    vertexSize += 2 * sizeof(float);
  }
  if(format & R_VB_NORMAL)
  {
    vertexSize += 3 * sizeof(float);
  }
  if(format & R_VB_TANGENT)
  {
    vertexSize += 3 * sizeof(float);
  }
  if(format & R_VB_BITANGENT)
  {
    vertexSize += 3 * sizeof(float);
  }
  if(format & R_VB_COLOR)
  {
    vertexSize += sizeof(int);
  }

  unsigned int offset = vbOffset;
  if(format & R_VB_XYZ)
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, vertexSize, (char*)0 + offset);
    offset += 3 * sizeof(float);
  }

  if(format & R_VB_TC0)
  {
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, vertexSize, (char*)0 + offset);
    offset += 2 * sizeof(float);
  }

  if(format & R_VB_NORMAL)
  {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, vertexSize, (char*)0 + offset);
    offset += 3 * sizeof(float);
  }

  if(format & R_VB_TANGENT)
  {
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_FLOAT, vertexSize, (char*)0 + offset);
    offset += 3 * sizeof(float);
  }

  if(format & R_VB_BITANGENT)
  {
    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_FLOAT, vertexSize, (char*)0 + offset);
    offset += 3 * sizeof(float);
  }

  if(format & R_VB_COLOR)
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, vertexSize, (char*)0 + offset);
    offset += 4 * sizeof(char);
  }


  switch(mode)
  {
  case R_TRIANGLES:
    mode = GL_TRIANGLES;
    break;

  case R_TRIANGLE_STRIP:
    mode = GL_TRIANGLE_STRIP;
    break;

  case R_LINES:
    mode = GL_LINES;
    break;

  case R_LINE_STRIP:
    mode = GL_LINE_STRIP;
    break;

  case R_POINTS:
    mode = GL_POINTS;
    break;

  default:
    break;
  }




  if(mode)
  {
    if(ib)
    {
      glDrawElements(mode, nElements, GL_UNSIGNED_INT, (char*)ibOffset);
    }
    else
    {
      glDrawArrays(mode, 0, nElements);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}



// @todo delete vertex buffer data in renderer or no?
void GLRenderDevice::DestroyVB(unsigned int vb)
{
}


// @todo delete vertex buffer data in renderer or no?
void GLRenderDevice::DestroyIB(unsigned int ib)
{
}

