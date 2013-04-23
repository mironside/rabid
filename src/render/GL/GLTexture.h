// GLTexture.h
#ifndef __GL_TEXTURE__
#define __GL_TEXTURE__

#include "render/materialSystem/texture.h"

class GLTexture : public Texture
{
protected:
  unsigned int glTex;
  unsigned int _Load(unsigned char* data);
  float* texData;

public:
  GLTexture();
  virtual ~GLTexture();

  unsigned int GetGLType();
  unsigned int GetGLFormat();
  unsigned int GetGLHandle() { return glTex; }

  void BeginRenderTarget();
  void EndRenderTarget(int w, int h);

  float* GetData();
};

#endif
