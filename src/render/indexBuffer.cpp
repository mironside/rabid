// indexBuffer.cpp
#include "render/indexBuffer.h"


IndexBuffer::IndexBuffer(unsigned int _format, unsigned int _nIndices) :
  HardwareBuffer(0/*static*/), format(_format), nIndices(_nIndices)
{
  if(format == IBF_UINT)
    size = sizeof(int) * nIndices;
}

