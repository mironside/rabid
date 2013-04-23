/**
  @file renderDevice.h
  @author Christopher Olsen
  @date Tue Oct  5 13:05:53 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __RENDER_DEVICE__
#define __RENDER_DEVICE__

#include "libsn/list.h"

class HardwareBufferMgr;
class Texture;
class ShaderProgram;
class RenderState;
class TextureState;
class VertexBuffer;
class IndexBuffer;

class IRenderDevice
{
protected:
  snList<RenderState*> renderStates;
  snList<TextureState*> textureStates;

public:
  IRenderDevice() {};
  virtual ~IRenderDevice() {};

  virtual void Create(int arg1, int arg2, int arg3) = 0;
  virtual void Destroy() = 0;

  // render states
  virtual void Clear(unsigned int buffers) = 0;
  virtual void SetClearColor(float r, float g, float b, float a) = 0;
  virtual void SetViewport(int x, int y, int w, int h) = 0;
  virtual void SetFrustum(float l, float r, float b, float t, float n, float f) = 0;
  virtual void SetViewMatrix(float* matrix) = 0;
  virtual void MultViewMatrix(float* matrix) = 0;
  virtual void PopViewMatrix() = 0;
  virtual void SetProjectionMatrix(float* matrix) = 0;
  virtual void Flip() = 0;
  virtual void DrawLine(float x1, float y1, float z1,
                        float x2, float y2, float z2) = 0;
  virtual void SetColor(float r, float g, float b) = 0;
  virtual void SetLight(unsigned int light, float x, float y, float z) = 0;
  virtual void SetLightDir(unsigned int light, float x, float y, float z) = 0;
  virtual void SetLightIntensity(unsigned int light, float I) = 0;
  virtual void SetLightFraction(unsigned int light, float f) = 0;
  virtual void DrawPoint(float size, float x, float y, float z) = 0;
  virtual void DrawRect(float x, float y, float sx, float sy, float tx, float ty, float stx, float sty) = 0;



  // Basic Rendering
  virtual void DrawRect(float x, float y, float w, float h) = 0;
  virtual void DrawCoordinateAxis() = 0;

  // textures
  virtual void DisableTextureStage(int stage) = 0;
  virtual void EnableTextureStage(int stage) = 0;
  virtual Texture* CreateTexture(unsigned int width, unsigned int height,
                                 unsigned int format, unsigned int nMips,
                                 unsigned char* data) = 0;
  virtual void BindTexture(unsigned int textureStage, Texture* tex) = 0;
  virtual void FreeAllTextures() = 0;

  // shader programs
  virtual ShaderProgram* CreateShaderProgram() = 0;
  virtual void BindShaderProgram(ShaderProgram* s) = 0;
  virtual void DisableAllTextureStages() = 0;
  virtual void SetUniform(ShaderProgram* s, const char* name, float v) = 0;



  // render state
  RenderState* GetRenderState(int _srcBlend, int _dstBlend, int _blendFunc, int _alphaTest,
                              float _alphaRef, int _depthTest, int _depthWrite, int _cullMode);

  virtual RenderState* CreateRenderState(int _srcBlend, int _dstBlend, int _blendFunc, int _alphaTest,
                                         float _alphaRef, int _depthTest, int _depthWrite, int _cullMode) = 0;

  virtual void BindRenderState(RenderState* rs) = 0;


  // texture state
  TextureState* GetTextureState(int _colorCombiner, int _alphaCombiner,
                                int _wrapMode, int _filterMode);

  virtual TextureState* CreateTextureState(int _colorCombiner, int _alphaCombiner,
                                           int _wrapMode, int _filterMode) = 0;

  virtual void BindTextureState(int stage, TextureState* ts) = 0;

  virtual VertexBuffer* CreateVertexBuffer(unsigned int format, unsigned int nVertices, unsigned int usage) = 0;
  virtual IndexBuffer*  CreateIndexBuffer(unsigned int format, unsigned int nIndices) = 0;
};



#endif


