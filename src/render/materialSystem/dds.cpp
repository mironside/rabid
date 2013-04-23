// dds.cpp
#include "dds.h"
#include "render/renderer_defs.h"
#include "common/global.h"
#include "texture.h"

#define DDSD_CAPS        0x00000001
#define DDSD_HEIGHT      0x00000002
#define DDSD_WIDTH       0x00000004
#define DDSD_PITCH       0x00000008
#define DDSD_PIXELFORMAT 0x00001000
#define DDSD_MIPMAPCOUNT 0x00020000
#define DDSD_LINEARSIZE  0x00080000
#define DDSD_DEPTH       0x00800000

#define DDPF_ALPHAPIXELS 0x00000001
#define DDPF_FOURCC      0x00000004
#define DDPF_RGB         0x00000040

#define DDSCAPS_COMPLEX  0x00000008
#define DDSCAPS_TEXTURE  0x00001000
#define DDSCAPS_MIPMAP   0x00400000

#define DDSCAPS2_CUBEMAP           0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000
#define DDSCAPS2_VOLUME            0x00200000

#define DXT1 0x31545844
#define DXT3 0x33545844
#define DXT5 0x35545844


typedef struct ddsCaps2_s
{
  unsigned int caps1;  // DDSCAPS_TEXTURE, DDSCAPS_MIPMAP, DDSCAPS_COMPLEX
  unsigned int caps2;  // for cubemaps
  unsigned int reserved[2];
} ddsCaps2_t;


typedef struct ddPixelFormat_s
{
  unsigned int size;
  unsigned int flags;  // DDPF_RGB for uncompressed,
                       // DDPF_FOURCC for compressed
  unsigned int fourCC;  // DDPF_FOURCC
  unsigned int rgbBitCount; // bits per pixel
  unsigned int rBitMask; // mask to R component (0xFF000000)
  unsigned int gBitMask; // mask to G component (0x00FF0000)
  unsigned int bBitMask; // mask to B component (0x0000FF00)
  unsigned int aBitMask; // mask to A component (0x000000FF)
} ddPixelFormat_t;


typedef struct ddsd2_s
{
  unsigned int size;     // should be 124
  unsigned int flags;    // DDSD_CAPS, DDSD_PIXELFORMAT,
                         // DDSD_WIDTH, DDSD_HEIGHT
  unsigned int height;
  unsigned int width;
  unsigned int pitch;    // line size for uncompressed images (DDSD_PITCH),
                         // size of main image for compressed (DDSD_LINEARSIZE)
  unsigned int depth;    // depth for volume textures (DDSD_DEPTH)
  unsigned int mipCount; // DDSD_MIPMAPCOUNT
  unsigned int reserved1[11];  // unused
  ddPixelFormat_t pixelFormat; 
  ddsCaps2_t ddCaps;
  unsigned int reserved2;  // unused
} ddsd2_t;

typedef struct ddsHeader_s
{
  unsigned int magic;
  ddsd2_t ddsd;
} ddsHeader_t;


unsigned char* ReadDDS(unsigned char* buffer, unsigned int* width,
                       unsigned int* height, unsigned int* format)
{
  ddsHeader_t* header = (ddsHeader_t*)buffer;
  *width = header->ddsd.width;
  *height = header->ddsd.height;

  if(header->ddsd.pixelFormat.flags & DDPF_RGB)
  {
    // uncompressed formats
    if(header->ddsd.pixelFormat.rgbBitCount == 32)
    {
      // RGBA
      if(header->ddsd.pixelFormat.rBitMask == 0x00FF0000 &&
         header->ddsd.pixelFormat.gBitMask == 0x0000FF00 &&
         header->ddsd.pixelFormat.bBitMask == 0x000000FF &&
         header->ddsd.pixelFormat.aBitMask == 0xFF000000)
      {
        *format = PF_B8G8R8A8;
      }
      // BGRA
      else if(header->ddsd.pixelFormat.rBitMask == 0x000000FF &&
              header->ddsd.pixelFormat.gBitMask == 0x0000FF00 &&
              header->ddsd.pixelFormat.bBitMask == 0x00FF0000 &&
              header->ddsd.pixelFormat.aBitMask == 0xFF000000)
      {
        *format = PF_R8G8B8A8;
      }
    }
    else if(header->ddsd.pixelFormat.rgbBitCount == 24)
    {
      // RGB
      if(header->ddsd.pixelFormat.rBitMask == 0x00FF0000 &&
         header->ddsd.pixelFormat.gBitMask == 0x0000FF00 &&
         header->ddsd.pixelFormat.bBitMask == 0x000000FF)
      {
        *format = PF_R8G8B8;
      }
      // BGR
      else if(header->ddsd.pixelFormat.rBitMask == 0x000000FF &&
              header->ddsd.pixelFormat.gBitMask == 0x0000FF00 &&
              header->ddsd.pixelFormat.bBitMask == 0x00FF0000)
      {
        *format = PF_B8G8R8;
      }
    }
  }
  else if(header->ddsd.pixelFormat.fourCC == DXT1)
  {
    *format = PF_DXT1;
  }
  else if(header->ddsd.pixelFormat.fourCC == DXT3)
  {
    *format = PF_DXT3;
  }
  else if(header->ddsd.pixelFormat.fourCC == DXT5)
  {
    *format = PF_DXT5;
  }

  return (buffer + sizeof(ddsHeader_t));
}

