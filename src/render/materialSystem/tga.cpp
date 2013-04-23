// tga.cpp
#include "render/renderer_defs.h"
#include "common/global.h"
#include "texture.h"

typedef struct tgaHeader_s
{
  unsigned char header[12];
  unsigned short width;
  unsigned short height;
  unsigned char bpp;
  unsigned char flags;
} tgaHeader_t;

unsigned char* ReadTGA(unsigned char* buffer, unsigned int* width,
                       unsigned int* height, unsigned int* format)
{
  tgaHeader_t* header = (tgaHeader_t*)buffer;
  *width = header->width;
  *height = header->height;

  if(header->bpp == 32)
    *format = PF_B8G8R8A8;
  else if(header->bpp == 24)
    *format = PF_B8G8R8;
  else
    *format = 0;

  return (buffer + sizeof(tgaHeader_t));
}

