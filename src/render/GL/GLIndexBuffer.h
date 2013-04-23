// GLIndexBuffer.h
#ifndef __GL_INDEX_BUFFER__
#define __GL_INDEX_BUFFER__

#include "render/indexBuffer.h"

class GLIndexBuffer : public IndexBuffer
{
protected:
  unsigned int glIb;

  void* _Lock(unsigned int lockType);
  void  _Unlock();

public:
  GLIndexBuffer(unsigned int _format, unsigned int _nIndices);
  virtual ~GLIndexBuffer();
  unsigned int GetGLHandle() { return glIb; }
  unsigned int GetGLFormat();
};

#endif
