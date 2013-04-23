// GLTexture.cpp
#include "GLTexture.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

#ifdef WIN32
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
#endif


GLTexture::GLTexture() :
  Texture(), glTex(0), texData(0)
{
}



GLTexture::~GLTexture()
{
  glDeleteTextures(1, &glTex);
}



unsigned int GLTexture::_Load(unsigned char* data)
{
  glGenTextures(1, &glTex);
  glBindTexture(GL_TEXTURE_2D, glTex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  if(!data)
    glTexImage2D(GetGLType(), 0, 4, width, height, 0, GetGLFormat(), GL_UNSIGNED_BYTE, data);
  else
    gluBuild2DMipmaps(GetGLType(), 4, width, height, GetGLFormat(), GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  return true;
}



unsigned int GLTexture::GetGLType()
{
  switch(type)
  {
  case TEX_1D:      return GL_TEXTURE_1D;
  case TEX_2D:      return GL_TEXTURE_2D;
  case TEX_CUBEMAP: return GL_TEXTURE_CUBE_MAP;
  default:          break;
  }

  return 0;
}



unsigned int GLTexture::GetGLFormat()
{
  switch(format)
  {
  case PF_R8G8B8:   return GL_RGB;
  case PF_B8G8R8:   return GL_BGR;
  case PF_R8G8B8A8: return GL_RGBA;
  case PF_B8G8R8A8: return GL_BGRA;
  case PF_DXT1:     return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
  case PF_DXT3:     return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
  case PF_DXT5:     return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
  case PF_FLOAT32_RGBA: return GL_RGBA_FLOAT32_ATI;
  default:          break;
  }

  return 0;
}



void GLTexture::BeginRenderTarget()
{
  glViewport(0,0, width,height);
}


void GLTexture::EndRenderTarget(int w, int h)
{
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, GetGLHandle());
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0,0, width,height, 0);
  glViewport(0,0, w,h);
}



float* GLTexture::GetData()
{
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, GetGLHandle());
  if(!texData)
    texData = new float[width*height*4];
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, texData);
  return texData;
}

