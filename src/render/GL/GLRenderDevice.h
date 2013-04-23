/**
  @file GLRenderDevice.h
  @author 
  @date Thu Sep  9 00:45:19 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __GL_RENDER_DEVICE__
#define __GL_RENDER_DEVICE__

#include "render/renderDevice.h"

#define MAX_TEXTURES 256
#define MAX_SHADER_PROGRAMS 256
#define MAX_VERTEX_BUFFERS 256
class HardwareBufferMgr;
class GLHardwareBufferMgr;
class ShaderProgram;


class GLRenderDevice : public IRenderDevice
{
protected:
  GLHardwareBufferMgr* hwBufferMgr;
  unsigned int apiVertexBuffers[MAX_VERTEX_BUFFERS];
  float light[3];


public:
  GLRenderDevice();
  virtual ~GLRenderDevice();

  // platform specific
  void Create(int arg1, int arg2=0, int arg3=0);
  void Destroy();
  void Flip();
  void InitializeExtensions();


  HardwareBufferMgr* GetHardwareBufferMgr()
  { return (HardwareBufferMgr*)hwBufferMgr; }



  char* GetAPI() { return "OpenGL"; }

  // render states
  void SetClearColor(float r, float g, float b, float a);
  void Clear(unsigned int buffers);
  void SetViewMatrix(float* matrix);
  void MultViewMatrix(float* matrix);
  void PopViewMatrix();
  void SetProjectionMatrix(float* matrix);
  void SetViewport(int x, int y, int w, int h);
  void SetFrustum(float l, float r, float b, float t, float n, float f);

  void DrawLine(float x1, float y1, float z1,
                float x2, float y2, float z2);

  void SetColor(float r, float g, float b);
  void SetLight(unsigned int light, float x, float y, float z);
  void SetLightDir(unsigned int light, float x, float y, float z);
  void SetLightIntensity(unsigned int light, float I);
  void SetLightFraction(unsigned int light, float f);

  void DrawPoint(float size, float x, float y, float z);
  void DrawRect(float x, float y, float sx, float sy, float tx, float ty, float stx, float sty);
  void DrawRect(float s, float x, float y, float z);


  void DrawCoordinateAxis();



  // Texture
  Texture* CreateTexture(unsigned int width, unsigned int height,
                         unsigned int format, unsigned int nMips,
                         unsigned char* data);
  void BindTexture(unsigned int textureStage, Texture* tex);
  void FreeAllTextures();
  void DisableTextureStage(int stage);
  void EnableTextureStage(int stage);
  void DisableAllTextureStages();
  void SetUniform(ShaderProgram* s, const char* name, float v);



  // hardware shaders
  ShaderProgram* CreateShaderProgram();
  void BindShaderProgram(ShaderProgram* s);


  // render state
  RenderState* CreateRenderState(int _srcBlend, int _dstBlend, int _blendFunc, int _alphaTest,
                                 float _alphaRef, int _depthTest, int _depthWrite, int _cullMode);

  void BindRenderState(RenderState* rs);

  TextureState* CreateTextureState(int _colorCombiner, int _alphaCombiner,
                                   int _wrapMode, int _filterMode);
  void BindTextureState(int stage, TextureState* ts);





  VertexBuffer* CreateVertexBuffer(unsigned int format, unsigned int nVertices, unsigned int usage);
  IndexBuffer*  CreateIndexBuffer(unsigned int format, unsigned int nIndices);
};

#endif

