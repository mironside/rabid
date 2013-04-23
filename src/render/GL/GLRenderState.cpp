/**
  @file GLRenderState.cpp
  @author Christopher Olsen
  @date Mon Apr 11 01:16:47 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLRenderState.h"
#include "render/renderer_defs.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifndef WIN32
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

#ifdef WIN32
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
#endif

RenderState* RenderState::curState = 0;

int GetGLBlend(int b)
{
  switch(b)
  {
  case R_ONE: return GL_ONE;
  case R_ZERO: return GL_ZERO;

  case R_SRC_ALPHA: return GL_SRC_ALPHA;
  case R_DST_ALPHA: return GL_DST_ALPHA;
  case R_SRC_COLOR: return GL_SRC_COLOR;
  case R_DST_COLOR: return GL_DST_COLOR;

  case R_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
  case R_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
  case R_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
  case R_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
  }

  return GL_ONE;
}


int GetGLBlendFunc(int b)
{
  switch(b)
  {
  case R_ADD: return GL_FUNC_ADD;
  case R_SUBTRACT: return GL_FUNC_SUBTRACT;
  }

  return GL_FUNC_ADD;
}


int GetGLTest(int t)
{
  switch(t)
  {
  case R_GT: return GL_GREATER;
  case R_LT: return GL_LESS;
  case R_GTE: return GL_GEQUAL;
  case R_LTE: return GL_LEQUAL;
  case R_EQ: return GL_EQUAL;
  case R_NEQ: return GL_NOTEQUAL;
  }

  return GL_ALWAYS;
}

int GetGLEnable(int e)
{
  switch(e)
  {
  case R_ENABLE: return GL_TRUE;
  case R_DISABLE: return GL_FALSE;
  }

  return 1;
}

int GetGLCull(int c)
{
  switch(c)
  {
  case R_CULL_FRONT: return GL_FRONT;
  case R_CULL_BACK: return GL_BACK;
  }

  return GL_BACK;
}



void SetBlend(int src, int dst)
{
  glEnable(GL_BLEND);
  if(!src && ! dst)
  {
    glBlendFunc(GL_ONE, GL_ZERO);
    return;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GetGLBlend(src), GetGLBlend(dst));
}


void SetBlendFunc(int bf)
{
  glBlendEquation(GetGLBlendFunc(bf));
}


void SetAlphaTest(int at, float r)
{
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GetGLTest(at), r);
}


void SetDepthTest(int dt)
{
  if(!dt)
    glDisable(GL_DEPTH_TEST);
  else
    glEnable(GL_DEPTH_TEST);
  glDepthFunc(GetGLTest(dt));
}

void SetDepthWrite(int dw)
{
  dw = GetGLEnable(dw);
  glDepthMask(dw);
}

void SetCull(int c)
{
  if(c == R_CULL_NONE)
    glDisable(GL_CULL_FACE);
  else
    glEnable(GL_CULL_FACE);

  glFrontFace(GL_CW);
  glCullFace(GetGLCull(c));
}




void GLRenderState::Bind()
{
  SetBlend(srcBlend, dstBlend);
  SetBlendFunc(blendFunc);
  SetAlphaTest(alphaTest, alphaRef);
  SetCull(cullMode);
  SetDepthTest(depthTest);
  SetDepthWrite(depthWrite);

/*
  if(!curState)
  {

    curState = this;
    return;
  }

  if(curState->GetSrcBlend()  != srcBlend ||
     curState->GetDstBlend()  != dstBlend)
  {
    SetBlend(srcBlend, dstBlend);
  }

  if(curState->GetBlendFunc() != blendFunc)
  {
    SetBlendFunc(blendFunc);
  }

  if(curState->GetAlphaTest() != alphaTest ||
     curState->GetAlphaRef()   != alphaRef)
  {
    SetAlphaTest(alphaTest, alphaRef);
  }

  if(curState->GetDepthTest() != depthTest)
  {
    SetDepthTest(depthTest);
  }

  if(curState->GetDepthWrite() != depthWrite)
  {
    SetDepthWrite(depthWrite);
  }

  if(curState->GetCullMode() != cullMode)
  {
    SetCull(cullMode);
  }
*/
  curState = this;
}

