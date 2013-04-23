/**
@file renderer.cpp
@author Christopher Olsen
@date Thu Sep 23 00:27:28 2004

Copyright (C) 2004 Christopher A Olsen

@todo Binding Shader 0 should disbale all texture stages and set
default render states

@todo Should support changing of the rendering device (destroy,
recreate, and call reload on shaders and textures)
*/
#include "renderer.h"
#include "materialSystem/materialSystem.h"
#include "meshSystem/meshSystem.h"
#include "meshSystem/mesh.h"
#include "render/hardwareBufferMgr.h"
#include <string.h>

#include <stdio.h>
#include <stdarg.h>

#include "renderDevice.h"
#include "common/log.h"
#include "common/global.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

IRenderDevice* g_renderDevice;

#ifdef WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C"
#endif

typedef void (*GETRENDERDEVICE)(IRenderDevice**);



Renderer::Renderer()
{
}


void Renderer::Create(const char* API, int arg1, int arg2, int arg3)
{
  g_renderDevice = 0;
  if(strcmp(API, "GL") == 0)
  {
#ifdef WIN32
    HMODULE hDll = LoadLibrary("GL.dll");
    GETRENDERDEVICE GetRenderDevice = (GETRENDERDEVICE)GetProcAddress(hDll, "GetRenderDevice");
    GetRenderDevice(&g_renderDevice);
#else
    char library[256];
    getcwd(library, 256);
    sprintf(library, "%s/libGL.so", library);
    void* hLib = dlopen(library, RTLD_LAZY);
    if(!hLib)
    {
      printf("%s\n", dlerror());
      return;
    }

    GETRENDERDEVICE GetRenderDevice = (GETRENDERDEVICE)dlsym(hLib, "GetRenderDevice");

    if(!GetRenderDevice)
    {
      printf("%s\n", dlerror());
      return;
    }

    GetRenderDevice(&g_renderDevice);

#endif
  }
  else
  {
#ifdef WIN32
    HMODULE hDll = LoadLibrary("DX.dll");
    GETRENDERDEVICE GetRenderDevice = (GETRENDERDEVICE)GetProcAddress(hDll, "GetRenderDevice");
    GetRenderDevice(&g_renderDevice);
#endif
  }

  g_renderDevice->Create(arg1,arg2,arg3);
  LOG(0, "renderDevice wndCreate");


  SetViewport(0, 0, viewport[2] - viewport[0], viewport[3] - viewport[1]);

  if(!g_materialSystem)
    g_materialSystem = new MaterialSystem();
  if(!g_meshSystem)
    g_meshSystem = new MeshSystem();

  g_materialSystem->Initialize();
  g_meshSystem->Initialize();

  textShader = GetMaterial("console_text");
}



Renderer::~Renderer()
{
  LOG_STOP();
  delete g_materialSystem;
  g_materialSystem = 0;
  delete g_meshSystem;
  g_renderDevice->Destroy();
}



void Renderer::SetViewport(unsigned int x, unsigned int y,
                           unsigned int w, unsigned int h)
{
  FN(0, "Renderer::SetViewPort");
  g_renderDevice->SetViewport(x, y, w, h);
  viewport[0] = x;
  viewport[1] = y;
  viewport[2] = w;
  viewport[3] = h;
}


void Renderer::SetFrustum(float l, float r, float b, float t, float n, float f)
{
  g_renderDevice->SetFrustum(l, r, b, t, n, f);
}



void Renderer::SetViewMatrix(float* matrix)
{
  FN(0, "Renderer::SetViewMatrix");
  g_renderDevice->SetViewMatrix(matrix);
}



void Renderer::SetProjectionMatrix(float* matrix)
{
  FN(0, "Renderer::SetProjectionMatrix");

  if(!matrix)
  {
    float fov = 45.0f;
    float nearplane = 1.0f;
    float farplane = 1000.0f;
    float top;
    float bottom;
    float left;
    float right;
    float aspect = (float)viewport[2] / (float)viewport[3];

    top = tanf(fov * 3.14159f / 360.0f) * nearplane;
    bottom = -top;

    left = aspect * bottom;
    right = aspect * top;

    float mat[16] = {2.0f*nearplane / (right - left), 0, 0, 0,
      0, 2*nearplane / (top - bottom), 0, 0,
      (right+left) / (right-left), (top+bottom) / (top-bottom), -farplane / (farplane-nearplane), -1.0f,
      0, 0, -2*farplane*nearplane / (farplane-nearplane), 0};

    g_renderDevice->SetProjectionMatrix(mat);
  }
  else
  {
    g_renderDevice->SetProjectionMatrix(matrix);
  }
}



void Renderer::MultViewMatrix(float* matrix)
{
  g_renderDevice->MultViewMatrix(matrix);
}


void Renderer::PopViewMatrix()
{
  g_renderDevice->PopViewMatrix();
}


void Renderer::DrawRect(float x, float y, float sx, float sy, float tx, float ty, float stx, float sty)
{
  g_renderDevice->DrawRect(x, y, sx, sy, tx, ty, stx, sty);
}


void Renderer::SetClearColor(float r, float g, float b, float a)
{
  FN(0, "Renderer::SetClearColor");
  g_renderDevice->SetClearColor(r, g, b, a);
}


void Renderer::Clear(unsigned int buffer)
{
  FN(0, "Renderer::Clear");
  g_renderDevice->Clear(buffer);
}



void Renderer::DrawCoordinateAxis()
{
  FN(0, "Renderer::DrawCoordinateAxis");
  g_renderDevice->DrawCoordinateAxis();
}



unsigned int Renderer::GetVertexSize(unsigned int format)
{
  FN(0, "Renderer::GetVertexSize");
  int size = 0;

  if(format & R_VB_XYZ)
    size += 3 * sizeof(float);
  if(format & R_VB_TC0)
    size += 2 * sizeof(float);
  if(format & R_VB_NORMAL)
    size += 3 * sizeof(float);
  if(format & R_VB_TANGENT)
    size += 3 * sizeof(float);
  if(format & R_VB_BITANGENT)
    size += 3 * sizeof(float);
  if(format & R_VB_COLOR)
    size += sizeof(int);

  return size;
}



void Renderer::Flip()
{
  FN(0, "Renderer::Flip");
  g_renderDevice->Flip();
}


void Renderer::DrawLine(float x1, float y1, float z1,
                        float x2, float y2, float z2)
{
  FN(0, "Renderer::DrawLine");
  g_renderDevice->DrawLine(x1, y1, z1, x2, y2, z2);
}



void Renderer::SetColor(float r, float g, float b)
{
  FN(0, "Renderer::SetColor");
  //printf("SetColor");
  g_renderDevice->SetColor(r, g, b);
}


void Renderer::SetLight(unsigned int light, float x, float y, float z)
{
  FN(0, "Renderer::SetLight");
  g_renderDevice->SetLight(light, x, y, z);
}


void Renderer::DrawPoint(float size, float x, float y, float z)
{
  FN(0, "Renderer::DrawPoint");
  g_renderDevice->DrawPoint(size, x, y, z);
}



void Renderer::DrawTextP(float x, float y, float size, char* string, ...)
{
  char text[2048];
  va_list ap;

  if(string == NULL)
    return;

  va_start(ap, string);
  vsprintf(text, string, ap);
  va_end(ap);

  DrawTextLen(x, y, size, text, (int)strlen(text));
}


/*
gui/themes/grey/grey.def

- find last '/'
- find next to last '/'
*/

Material* Renderer::GetMaterial(char* materialName)
{
  return g_materialSystem->GetMaterial(materialName);
}



void Renderer::BindMaterial(Material* m)
{
  g_materialSystem->BindMaterial(m);
}



void Renderer::ReloadMaterial(int materialId)
{
  g_materialSystem->ReloadMaterial(materialId);
}



void Renderer::SetOrthoProjection()
{
  float matrix[16] = {2.0f / (viewport[2] - viewport[0]), 0, 0, 0,
                      0, 2.0f / (viewport[3] - viewport[1]), 0, 0,
                      0, 0, 2.0f / 100.0f, 0,
                      -1,-1,-1,1};

  g_renderDevice->SetProjectionMatrix(matrix);
}



void Renderer::DrawTextLen(float x, float y, float size, char* str, int len)
{
  g_materialSystem->BindMaterial(textShader);
  SetColor(1,1,1);
  int startX = (int)x;
  float tx = 0, ty = 0;
  for(int i = 0; i < len; i++)
  {
    int c = str[i];
    if(c < 32)
    {
      switch(c)
      {
      case 1:  SetColor(0,0,0);  break;
      case 2:  SetColor(0,0,1);  break;
      case 3:  SetColor(0,1,0);  break;
      case 4:  SetColor(0,1,1);  break;
      case 5:  SetColor(1,0,0);  break;
      case 6:  SetColor(1,0,1);  break;
      case 7:  SetColor(1,1,0);  break;
      case 8:  SetColor(1,1,1);  break;
      case 9:  SetColor(0.85,0.85,1);  break;
      case 10: y -= size - 2.0f; x = startX; break;
      default: SetColor(0,0,0);  break;
      }
      continue;
    }

    c -= 32;
    if(c >= 0 && c < 96)
    {
      tx = (float)(c % 16) * (1.0f / 16.0f);
      ty = 1.0f - (((float)((c / 16)+1) * (1.0f / 16.0f)));
      DrawRect(x, y, size, size, tx, ty, 1.0f / 16.0f, 1.0f / 16.0f);
      x += size;
    }
  }
}



void Renderer::DrawMesh(Mesh* mesh)
{
  mesh->Draw();
}


void Renderer::DrawRect(float s, float x, float y, float z)
{
  g_renderDevice->DrawRect(s, x, y, z);
}


VertexBuffer* Renderer::CreateVertexBuffer(unsigned int format, unsigned int nVertices, unsigned int usage)
{
  return g_renderDevice->CreateVertexBuffer(format, nVertices, usage);
}


IndexBuffer*  Renderer::CreateIndexBuffer(unsigned int format, unsigned int nIndices)
{
  return g_renderDevice->CreateIndexBuffer(format, nIndices);
}


void Renderer::SetLightDir(unsigned int light, float x, float y, float z)
{
  g_renderDevice->SetLightDir(light, x,y,z);
}

void Renderer::SetLightIntensity(unsigned int light, float I)
{
  g_renderDevice->SetLightIntensity(light, I);
}

void Renderer::SetLightFraction(unsigned int light, float f)
{
  g_renderDevice->SetLightFraction(light, f);
}

  
void Renderer::SetUniform(ShaderProgram* s, const char* name, float v)
{
  g_renderDevice->SetUniform(s, name, v);
}

