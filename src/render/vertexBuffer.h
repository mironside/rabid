/**
  @file vertexBuffer.h
  @author Christopher Olsen
  @date Fri Apr  8 01:30:24 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __VERTEX_BUFFER__
#define __VERTEX_BUFFER__

#include "render/hardwareBuffer.h"

class IndexBuffer;

#define R_TRIANGLES      1
#define R_TRIANGLE_STRIP 2
#define R_QUADS          3
#define R_POINTS         4
#define R_LINES          5
#define R_LINE_STRIP     6


typedef enum
{
  VE_XYZ,
  VE_COLOR,
  VE_NORMAL,
  VE_TEXCOORD0,
  VE_TEXCOORD1,
  VE_TEXCOORD2,
  VE_TEXCOORD3,
  VE_TEXCOORD4,
  VE_TEXCOORD5,
  VE_TEXCOORD6,
  VE_TEXCOORD7
} eVertexElement;

/**
  Vertex Format:

  TexCoord component sizes (16 bits)
  blank (4 bits)
  num texcoords (4 bits)
  geom bits (8 bits)
*/
#define VF_XYZ     0x01
#define VF_COLOR   0x02
#define VF_NORMAL  0x04

#define VF_TEXMASK 0xf00
#define VF_TEX1 0x100
#define VF_TEX2 0x200
#define VF_TEX3 0x300
#define VF_TEX4 0x400
#define VF_TEX5 0x500
#define VF_TEX6 0x600
#define VF_TEX7 0x700
#define VF_TEX8 0x800

#define TEXCOORD2 0  // defaults to two component textures
#define TEXCOORD1 1  
#define TEXCOORD3 2
#define TEXCOORD4 3

#define VF_TC1(s) (TEXCOORD1 << (s*2+16))
#define VF_TC2(s) (TEXCOORD2)
#define VF_TC3(s) (TEXCOORD3 << (s*2+16))
#define VF_TC4(s) (TEXCOORD4 << (s*2+16))




class VertexBuffer : public HardwareBuffer
{
protected:
  unsigned int nVertices;
  unsigned int format;
  unsigned int vSize;

public:
  VertexBuffer(unsigned int _format, unsigned int _nVertices,
               unsigned int _usage);
  virtual ~VertexBuffer();

  unsigned int GetNumVertices() { return nVertices; }
  unsigned int GetVertexSize()  { return vSize; }
  unsigned int GetFormat()      { return format; }

  virtual void Render(unsigned int type, unsigned int nItems, unsigned int offset, 
                      IndexBuffer* ib) { };
};

inline unsigned int VertexSize(unsigned int format)
{
  int size = 0;

  if(format & VF_XYZ)
    size += 3 * sizeof(float);
  if(format & VF_COLOR)
    size += 4 * sizeof(float);
  if(format & VF_NORMAL)
    size += 3 * sizeof(float);

  unsigned int nTexStages = (VF_TEXMASK & format) >> 8;

  for(unsigned int i = 0; i < nTexStages; i++)
  {
    unsigned int tc = (format >> (i*2+16)) & 0x03;
    if(tc == TEXCOORD1)
      size += 1 * sizeof(float);
    else if(tc == TEXCOORD2)
      size += 2 * sizeof(float);
    else if(tc == TEXCOORD3)
      size += 3 * sizeof(float);
    else if(tc == TEXCOORD4)
      size += 4 * sizeof(float);
  }

  return size;
}


inline unsigned int VertexElementOffset(unsigned int format, eVertexElement e)
{
  int size = 0;
  if(e == VE_XYZ)
    return size;
  if(format & VF_XYZ)
    size += 3 * sizeof(float);

  if(e == VE_COLOR)
    return size;
  if(format & VF_COLOR)
    size += 4 * sizeof(float);

  if(e == VE_NORMAL)
    return size;
  if(format & VF_NORMAL)
    size += 3 * sizeof(float);

  unsigned int nTexStages = (VF_TEXMASK & format) >> 8;
  for(unsigned int i = 0; i < nTexStages; i++)
  {
    if(e == VE_TEXCOORD0 + i)
      return size;

    unsigned int tc = (format >> (i*2+16)) & 0x03;
    if(tc == TEXCOORD1)
      size += 1 * sizeof(float);
    else if(tc == TEXCOORD2)
      size += 2 * sizeof(float);
    else if(tc == TEXCOORD3)
      size += 3 * sizeof(float);
    else if(tc == TEXCOORD4)
      size += 4 * sizeof(float);
  }

  return -1;
}

#endif




