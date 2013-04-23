/**
  @file vertexBuffer.cpp
  @author Christopher Olsen
  @date Fri Apr  8 01:35:46 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "render/vertexBuffer.h"

VertexBuffer::VertexBuffer(unsigned int _format, unsigned int _nVertices,
                           unsigned int _usage) :
  HardwareBuffer(_usage), format(_format), nVertices(_nVertices)
{
  vSize = VertexSize(format);
  size = vSize * nVertices;
}



VertexBuffer::~VertexBuffer()
{
  // notify mgr of delete
}

