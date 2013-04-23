/**
  @file renderer.h
  @author 
  @date Thu Sep  9 00:44:17 2004

  Copyright (C) 2004 Christopher A Olsen

  Abstract interface for a renderer.

  @todo API RENDERER SHOULD BE A RENDER DEVICE INSIDE IRENDERER
*/
#ifndef __RENDER__
#define __RENDER__

#include "render/renderer_defs.h"
class HardwareBufferMgr;

class ShaderProgram;
class Material;
class Mesh;
class FileSystem;
class VertexBuffer;
class IndexBuffer;

typedef struct renderAPI_s
{
  FileSystem* fileSystem;
} renderAPI_t;

class IRenderer
{
protected:

public:
  virtual void Create(const char* API, int arg1, int arg2=0, int arg3=0) = 0;
  virtual void Release() = 0;
  
  // platform specific
  virtual void  Flip() = 0;
  virtual char* GetAPI() = 0;

  virtual void SetColor(float r, float g, float b) = 0;
  
  virtual void SetViewport(unsigned int x, unsigned int y,
                           unsigned int w, unsigned int h) = 0;
  virtual void SetFrustum(float l, float r, float b, float t,
                          float n, float f) = 0;
  virtual void SetOrthoProjection() = 0;
  virtual void SetViewMatrix(float* matrix) = 0;
  virtual void MultViewMatrix(float* matrix) = 0;
  virtual void PopViewMatrix() = 0;

  virtual void SetProjectionMatrix(float* matrix) = 0;
  virtual void SetClearColor(float r, float g, float b, float a) = 0;
  virtual void Clear(unsigned int buffers) = 0;

  virtual void DrawCoordinateAxis() = 0;
  virtual void DrawLine(float x1, float y1, float z1,
                float x2, float y2, float z2) = 0;
  virtual void SetLight(unsigned int light, float x, float y, float z) = 0;
  virtual void SetLightDir(unsigned int light, float x, float y, float z) = 0;
  virtual void SetLightIntensity(unsigned int light, float I) = 0;
  virtual void SetLightFraction(unsigned int light, float f) = 0;
  virtual void DrawPoint(float size, float x, float y, float z) = 0;
  virtual void DrawRect(float x, float y, float sx, float sy, float tx, float ty, float stx, float sty) = 0;
  virtual void DrawRect(float s, float x, float y, float z) = 0;
  virtual void DrawTextP(float x, float y, float size, char* str, ...) = 0;
  virtual void DrawTextLen(float x, float y, float size, char* str, int len) = 0;

  virtual Material* GetMaterial(char* materialName) = 0;
  virtual void BindMaterial(Material* m) = 0;
  virtual void ReloadMaterial(int materialId) = 0;
  virtual void SetUniform(ShaderProgram* s, const char* name, float v) = 0;

  // Vertex Buffers
public:
  virtual unsigned int GetVertexSize(unsigned int format) = 0;
  virtual void DrawMesh(Mesh* mesh) = 0;

  virtual VertexBuffer* CreateVertexBuffer(unsigned int format, unsigned int nVertices, unsigned int usage) = 0;
  virtual IndexBuffer*  CreateIndexBuffer(unsigned int format, unsigned int nIndices) = 0;
public:
};

#ifdef GAME
extern IRenderer* g_renderer;
#endif



#endif

