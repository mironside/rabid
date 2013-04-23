/**
  @file GLTextureState.cpp
  @author Christopher Olsen
  @date Mon Apr 11 02:19:58 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLTextureState.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include "glext.h"
#include "render/renderer_defs.h"
#include "common/log.h"

#ifdef WIN32
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
#endif

TextureState* TextureState::curState[16];

int GetGLCombiner(int c)
{
  switch(c)
  {
  case R_COMBINE_REPLACE:  return GL_REPLACE;
  case R_COMBINE_MODULATE: return GL_MODULATE;
  case R_COMBINE_ADD:      return GL_ADD;
  case R_COMBINE_SUBTRACT: return GL_SUBTRACT;
  case R_COMBINE_DOT3:     return GL_DOT3_RGB;
  case R_COMBINE_INTERPOLATE: return GL_INTERPOLATE;
  }

  return GL_MODULATE;
}


int GetGLWrap(int w)
{
  switch(w)
  {
  case R_WRAP_REPEAT: return GL_REPEAT;
  case R_WRAP_CLAMP:  return GL_CLAMP_TO_EDGE;
  }

  return GL_REPEAT;
}


int GetGLFilter(int f)
{
  switch(f)
  {
  case R_FILTER_LINEAR: return GL_LINEAR;
  case R_FILTER_NEAREST: return GL_NEAREST;
  }

  return GL_LINEAR;
}



void SetColorCombiner(int c)
{
  c = GetGLCombiner(c);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, c);

  if(c == GL_INTERPOLATE)
  {
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_ONE_MINUS_SRC_ALPHA);
  }
  else
  {
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
  }
}

void SetAlphaCombiner(int c)
{
  c = GetGLCombiner(c);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, c);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
}

void SetWrap(int w)
{
  w = GetGLWrap(w);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, w);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, w);
}

void SetFilter(int f)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilter(f));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilter(f));
}



void GLTextureState::Bind(int stage)
{
  glActiveTexture(GL_TEXTURE0 + stage);
  SetWrap(wrapMode);
  SetFilter(filterMode);
  SetColorCombiner(colorCombiner);
  SetAlphaCombiner(alphaCombiner);

/*
  if(!curState[stage])
  {
  }
  else if(curState[stage] != this)
  {
    if(curState[stage]->GetWrapMode() != wrapMode)
      SetWrap(wrapMode);

    if(curState[stage]->GetColorCombiner() != colorCombiner)
      SetColorCombiner(colorCombiner);

    if(curState[stage]->GetAlphaCombiner() != alphaCombiner)
      SetAlphaCombiner(alphaCombiner);

    if(curState[stage]->GetFilterMode() != filterMode)
      SetFilter(filterMode);
  }
*/
  curState[stage] = this;
}

