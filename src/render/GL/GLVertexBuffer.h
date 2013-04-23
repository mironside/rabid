/**
  @file GLVertexBuffer.h
  @author Christopher Olsen
  @date Fri Apr  8 01:16:08 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __GL_VERTEX_BUFFER__
#define __GL_VERTEX_BUFFER__

#include "render/vertexBuffer.h"

class IndexBuffer;

class GLVertexBuffer : public VertexBuffer
{
protected:
  unsigned int glVb;

  void* _Lock(unsigned int lockType);
  void  _Unlock();

public:
  GLVertexBuffer(unsigned int vSize, unsigned int nVertices,
                 unsigned int usage);
  virtual ~GLVertexBuffer();

  void Render(unsigned int type, unsigned int nItems,
              unsigned int offset, IndexBuffer* ib);
};

#endif
