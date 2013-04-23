/**
  @file hardwareBufferMgr.h
  @author Christopher Olsen
  @date Fri Apr  8 01:59:36 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __HARDWARE_BUFFER_MGR__
#define __HARDWARE_BUFFER_MGR__

#include "libsn/list.h"
class HardwareBuffer;
class VertexBuffer;
class IndexBuffer;


class HardwareBufferMgr
{
protected:
  static snList<HardwareBuffer*>* hwBuffers;

  virtual VertexBuffer* _CreateVertexBuffer(unsigned int vSize,
                                            unsigned int nVertices,
                                            unsigned int usage) = 0;
  virtual IndexBuffer* _CreateIndexBuffer(unsigned int vSize,
                                          unsigned int nVertices) = 0;

public:
  HardwareBufferMgr();
  virtual ~HardwareBufferMgr();

  VertexBuffer* CreateVertexBuffer(unsigned int vSize,
                                   unsigned int nVertices,
                                   unsigned int usage);

  IndexBuffer* CreateIndexBuffer(unsigned int vSize,
                                 unsigned int nVertices);
};

#endif
