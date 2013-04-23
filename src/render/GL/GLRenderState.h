/**
  @file GLRenderState.h
  @author Christopher Olsen
  @date Mon Apr 11 01:15:04 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __GL_RENDER_STATE__
#define __GL_RENDER_STATE__
#include "render/materialSystem/renderState.h"

class GLRenderState : public RenderState
{
protected:

public:
  GLRenderState(int _srcBlend, int _dstBlend, int _blendFunc, int _alphaTest,
                float _alphaRef, int _depthTest, int _depthWrite, int _cullMode) :
    RenderState(_srcBlend, _dstBlend, _blendFunc, _alphaTest,
                _alphaRef, _depthTest, _depthWrite, _cullMode) {   }
  
  virtual ~GLRenderState() { };

  void Bind();
};

#endif
