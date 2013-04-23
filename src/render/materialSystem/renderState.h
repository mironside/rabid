// renderStateMgr.h
#ifndef __RENDER_STATE_MGR__
#define __RENDER_STATE_MGR__

#include "common/handle.h"
#include "libsn/list.h"

class RenderState
{
protected:
  int srcBlend;
  int dstBlend;
  int blendFunc;
  int alphaTest;
  float alphaRef;
  int depthTest;
  int depthWrite;
  int cullMode;

  static RenderState* curState;


public:
  RenderState(int _srcBlend, int _dstBlend, int _blendFunc, int _alphaTest,
              float _alphaRef, int _depthTest, int _depthWrite, int _cullMode) :
    srcBlend(_srcBlend), dstBlend(_dstBlend), blendFunc(_blendFunc), alphaTest(_alphaTest),
    alphaRef(_alphaRef), depthTest(_depthTest), depthWrite(_depthWrite), cullMode(_cullMode)
  {  }
  
  virtual ~RenderState() { };

  virtual void Bind() = 0;

  int GetSrcBlend()   { return srcBlend; }
  int GetDstBlend()   { return dstBlend; }
  int GetBlendFunc()  { return blendFunc; }
  int GetAlphaTest()  { return alphaTest; }
  float GetAlphaRef() { return alphaRef; }
  int GetDepthTest()  { return depthTest; }
  int GetDepthWrite() { return depthWrite; }
  int GetCullMode()   { return cullMode; }
};

#endif
