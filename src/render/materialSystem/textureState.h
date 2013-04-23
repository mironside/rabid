/**
  @file textureStageMgr.h
  @author Christopher Olsen
  @date Mon Nov 29 02:02:37 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __TEXTURE_STAGE_MGR__
#define __TEXTURE_STAGE_MGR__

class TextureState
{
protected:
  int colorCombiner;
  int alphaCombiner;
  int wrapMode;
  int filterMode;

  static TextureState* curState[16];

public:
  TextureState(int _colorCombiner, int _alphaCombiner,
               int _wrapMode, int _filterMode) :
    colorCombiner(_colorCombiner), alphaCombiner(_alphaCombiner),
    wrapMode(_wrapMode), filterMode(_filterMode)
  {  }
  virtual ~TextureState() { };

  virtual void Bind(int stage) = 0;

  int GetColorCombiner() { return colorCombiner; }
  int GetAlphaCombiner() { return alphaCombiner; }
  int GetWrapMode()      { return wrapMode; }
  int GetFilterMode()    { return filterMode; }
};

#endif

