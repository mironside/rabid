/**
  @file hardwareBufferMgr.cpp
  @author Christopher Olsen
  @date Fri Apr  8 02:01:08 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "render/hardwareBufferMgr.h"
#include "libsn/list.h"


snList<HardwareBuffer*>* HardwareBufferMgr::hwBuffers = 0;



HardwareBufferMgr::HardwareBufferMgr()
{
  hwBuffers = new snList<HardwareBuffer*>();
}



HardwareBufferMgr::~HardwareBufferMgr()
{
}


VertexBuffer* HardwareBufferMgr::CreateVertexBuffer(unsigned int vSize,
                                                    unsigned int nVertices,
                                                    unsigned int usage)
{
  // test for size and whatnot

  VertexBuffer* vb = _CreateVertexBuffer(vSize, nVertices, usage);
  if(vb && hwBuffers)
    hwBuffers->PushBack((HardwareBuffer*)vb);

  return vb;
}



IndexBuffer* HardwareBufferMgr::CreateIndexBuffer(unsigned int format,
                                                  unsigned int nIndices)
{
  IndexBuffer* ib = _CreateIndexBuffer(format, nIndices);
  if(ib && hwBuffers)
    hwBuffers->PushBack((HardwareBuffer*)ib);

  return ib;
}

