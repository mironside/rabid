/**
  @file renderDevice.cpp
  @author Christopher Olsen
  @date Mon Apr 11 01:03:10 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "renderDevice.h"
#include "materialSystem/renderState.h"
#include "materialSystem/textureState.h"

RenderState* IRenderDevice::GetRenderState(int _srcBlend, int _dstBlend, int _blendFunc, int _alphaTest,
                                           float _alphaRef, int _depthTest, int _depthWrite, int _cullMode)
{
  snList<RenderState*>::Iterator it;
  for(it = renderStates.Begin(); it != renderStates.End(); ++it)
  {
    RenderState* rs = (*it);
    if((rs->GetSrcBlend()   == _srcBlend) &&
       (rs->GetDstBlend()   == _dstBlend) &&
       (rs->GetBlendFunc()  == _blendFunc) &&
       (rs->GetAlphaTest()  == _alphaTest) &&
       (rs->GetAlphaRef()   == _alphaRef) &&
       (rs->GetDepthTest()  == _depthTest) &&
       (rs->GetDepthWrite() == _depthWrite) &&
       (rs->GetCullMode()   == _cullMode))
    {
      return rs;
    }
  }

  RenderState* rs = CreateRenderState(_srcBlend, _dstBlend, _blendFunc, _alphaTest,
                                      _alphaRef, _depthTest, _depthWrite, _cullMode);
  renderStates.PushBack(rs);

  return rs;
}



TextureState* IRenderDevice::GetTextureState(int _colorCombiner, int _alphaCombiner,
                                             int _wrapMode, int _filterMode)
{
  snList<TextureState*>::Iterator it;
  for(it = textureStates.Begin(); it != textureStates.End(); ++it)
  {
    TextureState* ts = (*it);

    if(ts->GetColorCombiner() == _colorCombiner &&
       ts->GetAlphaCombiner() == _alphaCombiner &&
       ts->GetWrapMode() == _wrapMode &&
       ts->GetFilterMode() == _filterMode)
    {
      return ts;
    }
  }

  TextureState* ts = CreateTextureState(_colorCombiner, _alphaCombiner,
                                        _wrapMode, _filterMode);
  textureStates.PushBack(ts);
  return ts;
}


