// indexBuffer.h
#ifndef __INDEX_BUFFER__
#define __INDEX_BUFFER__

#include "render/hardwareBuffer.h"

#define IBF_UINT 1

class IndexBuffer : public HardwareBuffer
{
protected:
  unsigned int format;
  unsigned int nIndices;

public:
  IndexBuffer(unsigned int _format, unsigned int _nIndices);
  virtual ~IndexBuffer() { }

  unsigned int GetNumIndices() const
  { return nIndices; }
  unsigned int GetFormat() const
  { return format; }
};

#endif
