/**
  @file renderer.h
  @author 
  @date Thu Sep  9 00:44:17 2004

  Copyright (C) 2004 Christopher A Olsen

  Abstract interface for a renderer.

  @todo API RENDERER SHOULD BE A RENDER DEVICE INSIDE IRENDERER
*/
#ifndef __RENDERER__
#define __RENDERER__


#include "renderer_defs.h"
#include "public/render.h"
class Mesh;
class HardwareBufferMgr;
class Material;
class VertexBuffer;
class IndexBuffer;





#define MAX_VERTEX_BUFFERS  256
#define MAX_SHADERS         256
#define MAX_TEXTURES        256
#define MAX_SHADER_PROGRAMS 256
#define MAX_MESHES          256
#define MAX_MESH_INSTANCES  256


class IRenderDevice;


class Renderer : public IRenderer
{
protected:
  unsigned int viewport[4];
  unsigned int renderTarget;  // current render target
  Material* textShader;
  char API[16];



public:
  Renderer();
  virtual ~Renderer();


  void Create(const char* API, int arg1, int arg2=0, int arg3=0);
  void Release() { delete this; }

  // platform specific
  void  Flip();
  char* GetAPI() { return API; };

  void SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
  void SetFrustum(float l, float r, float b, float t, float n, float f);
  void SetOrthoProjection();
  void SetViewMatrix(float* matrix);
  void MultViewMatrix(float* matrix);
  void PopViewMatrix();
  void SetProjectionMatrix(float* matrix);
  void SetColor(float r, float g, float b);
  void SetClearColor(float r, float g, float b, float a);
  void Clear(unsigned int buffers);

  void DrawCoordinateAxis();
  void DrawLine(float x1, float y1, float z1,
                float x2, float y2, float z2);
  void SetLight(unsigned int light, float x, float y, float z);
  void SetLightDir(unsigned int light, float x, float y, float z);
  void SetLightIntensity(unsigned int light, float I);
  void SetLightFraction(unsigned int light, float f);
  void DrawPoint(float size, float x, float y, float z);
  void DrawRect(float x, float y, float sx, float sy, float tx, float ty, float stx, float sty);
  void DrawRect(float size, float x, float y, float z);
  void DrawTextP(float x, float y, float size, char* string, ...);
  void DrawTextLen(float x, float y, float size, char* str, int len);


  // textures
protected:

public:
  Material* GetMaterial(char* materialName);
  void BindMaterial(Material* m);
  void ReloadMaterial(int materilId);
  unsigned int GetVertexSize(unsigned int format);
  void SetUniform(ShaderProgram* s, const char* name, float v);


  void DrawMesh(Mesh* mesh);
  VertexBuffer* CreateVertexBuffer(unsigned int format, unsigned int nVertices, unsigned int usage);
  IndexBuffer*  CreateIndexBuffer(unsigned int format, unsigned int nIndices);
};

#endif

