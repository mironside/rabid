/**
  @file GLTextureState.h
  @author Christopher Olsen
  @date Mon Apr 11 02:17:57 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __GL_TEXTURE_STATE__
#define __GL_TEXTURE_STATE__

#include "render/materialSystem/textureState.h"

class GLTextureState : public TextureState
{
protected:

public:
  GLTextureState(int _colorCombiner, int _alphaCombiner,
                 int _wrapMode, int _filterMode) :
    TextureState(_colorCombiner, _alphaCombiner, _wrapMode, _filterMode)
  {  }

  virtual ~GLTextureState() { }

  void Bind(int stage);
};

#endif
