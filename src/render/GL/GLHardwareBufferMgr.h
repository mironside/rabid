/**
  @file GLHardwareBufferMgr.h
  @author Christopher Olsen
  @date Fri Apr  8 02:12:01 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "render/hardwareBufferMgr.h"

class VertexBuffer;
class IndexBuffer;

class GLHardwareBufferMgr : public HardwareBufferMgr
{
protected:
  VertexBuffer* _CreateVertexBuffer(unsigned int vSize,
                                    unsigned int nVertices,
                                    unsigned int usage);
  IndexBuffer* _CreateIndexBuffer(unsigned int format,
                                  unsigned int nIndices);

public:
  GLHardwareBufferMgr() : HardwareBufferMgr() { }
  virtual ~GLHardwareBufferMgr() { }
};

