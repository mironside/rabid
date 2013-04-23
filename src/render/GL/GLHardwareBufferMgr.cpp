/**
  @file GLHardwareBufferMgr.cpp
  @author Christopher Olsen
  @date Fri Apr  8 02:14:04 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLHardwareBufferMgr.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"


VertexBuffer* GLHardwareBufferMgr::_CreateVertexBuffer(unsigned int vSize,
                                                       unsigned int nVertices,
                                                       unsigned int usage)
{
  return static_cast<VertexBuffer*>(new GLVertexBuffer(vSize, nVertices, usage));
}



IndexBuffer* GLHardwareBufferMgr::_CreateIndexBuffer(unsigned int format,
                                                     unsigned int nIndices)
{
  return static_cast<IndexBuffer*>(new GLIndexBuffer(format, nIndices));
}

