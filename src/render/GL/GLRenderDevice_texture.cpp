/**
  @file GLRenderDevice_texture.cpp
  @author Christopher Olsen
  @date Fri Sep 17 00:17:15 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLRenderDevice.h"
#include "render/renderer_defs.h"
#include "GLTexture.h"
#include <stdio.h>
#include <string.h>
#ifndef WIN32
#define GL_GLEXT_PROTOTYPES
#endif
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

#ifdef WIN32
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2D;
#endif

Texture* GLRenderDevice::CreateTexture(unsigned int width, unsigned int height,
                                       unsigned int format, unsigned int nMips,
                                       unsigned char* data)
{
  Texture* tex = new GLTexture();
  if(!tex->Create(TEX_2D, (PixelFormat)format, width, height, nMips, data))
  {
    delete tex;
    tex = 0;
  }

  return tex;
}



void GLRenderDevice::BindTexture(unsigned int textureStage, Texture* tex)
{
  glActiveTexture(GL_TEXTURE0 + textureStage);
  glEnable(GL_TEXTURE_2D);

  if(!tex)
  {
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }

  GLTexture* glTex = (GLTexture*)tex;
  glBindTexture(GL_TEXTURE_2D, glTex->GetGLHandle());
}


void GLRenderDevice::FreeAllTextures()
{
}




void GLRenderDevice::DisableAllTextureStages()
{
  for(int i = 0; i < 16; i++)
  {
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }
}



void GLRenderDevice::DisableTextureStage(int stage)
{
  glActiveTexture(GL_TEXTURE0+stage);
  //  glClientActiveTexture(GL_TEXTURE0+stage);
  glDisable(GL_TEXTURE_2D);
}



void GLRenderDevice::EnableTextureStage(int stage)
{
  glActiveTexture(GL_TEXTURE0+stage);
  //  glClientActiveTexture(GL_TEXTURE0+stage);
  glEnable(GL_TEXTURE_2D);
}



