/**
  @file surface.cpp
  @author Christopher Olsen
  @date Fri Apr 29 15:58:28 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "render/surface.h"
#include "render/vertexBuffer.h"
#include "public/render.h"
#include "render/materialSystem/materialSystem.h"
#include "render/materialSystem/material.h"
#include "math/transformation.h"
#include "math/vector2.h"
#include "render/renderDevice.h"
#include "render/materialSystem/texture.h"
#include "windowedApp.h"
#include <assert.h>
#include <stdio.h>
extern IRenderDevice* g_renderDevice;
extern IRenderer* g_renderer;
extern MaterialSystem* g_materialSystem;

#define TEX_SIZE 16

/**
   @todo Using rectangular textures would be great for lightmaps
   @todo Use a standard lightmap resolution on all surfaces
*/


Surface::Surface() :
  vb(0), material(0), lightmap(0)
{
}



Surface::~Surface()
{
}



int Surface::Create(const char* matName, const snVector<Vector3f>& verts,
                    const snVector<Vector2f>& texcoords)
{
  type = S_NORMAL;
  vertices = verts;
  light = g_materialSystem->GetMaterial("lightmap");
  material = g_materialSystem->GetMaterial(matName, 1);
  vb = g_renderer->CreateVertexBuffer(VF_XYZ | VF_NORMAL | VF_TEX2 | VF_TC2(0) | VF_TC2(1),
                                      vertices.Size(), 0);

  unsigned char* p = new unsigned char[TEX_SIZE*TEX_SIZE*4];
  for(int i = 0; i < TEX_SIZE*TEX_SIZE*4; i++)
    p[i] = 0;

  // add RTT lightmap to the last stage of the material
  lightmap = g_renderDevice->CreateTexture(TEX_SIZE, TEX_SIZE, PF_R8G8B8A8, 1, p);
  printf("lightmap %p\n", lightmap);
  TextureState* ts = g_renderDevice->GetTextureState(R_COMBINE_MODULATE, R_COMBINE_MODULATE,
                                                     R_WRAP_CLAMP, R_FILTER_LINEAR);
  material->SetTextureState(1, ts);
  accum = g_renderDevice->CreateTexture(TEX_SIZE, TEX_SIZE, PF_R8G8B8A8, 1, p);
  resid = g_renderDevice->CreateTexture(TEX_SIZE, TEX_SIZE, PF_R8G8B8A8, 1, p);
  material->SetTexture(1, lightmap);




  blend = g_materialSystem->GetMaterial("blend", 1);
  blend->SetTexture(0, lightmap);
  blend->SetTextureState(0, ts);
  blend->SetTexture(1, accum);
  blend->SetTextureState(1, ts);

  rblend = g_materialSystem->GetMaterial("blend", 1);
  rblend->SetTexture(0, lightmap);
  rblend->SetTextureState(0, ts);
  rblend->SetTexture(1, resid);
  rblend->SetTextureState(1, ts);

  copy = g_materialSystem->GetMaterial("copy", 1);
  copy->SetTexture(0, accum);
  copy->SetTextureState(0, ts);

  rcopy = g_materialSystem->GetMaterial("copy", 1);
  rcopy->SetTexture(0, resid);
  rcopy->SetTextureState(0, ts);




  assert(vertices.Size() >= 3);

  // normal
  Vector3f v0 = Normalize(vertices[0] - vertices[1]);
  Vector3f v1 = Normalize(vertices[2] - vertices[1]);
  N = Cross(v0, v1);



  Vector2f tc0 = texcoords[0];
  Vector2f tc1 = texcoords[1];
  Vector2f tc2 = texcoords[2];

  // tangent
  double ds0 = tc0.y - tc1.y;
  double ds1 = tc2.y - tc1.y;
  T = Normalize(v0 * ds1 - v1 * ds0);

  // bitangent
  double dt0 = tc0.x - tc1.x;
  double dt1 = tc2.x - tc1.x;
  B = -Normalize(v0 * dt1 - v1 * dt0);

/*
  // tangent pointing the wrong way
  Vector3f n = Cross(T, B);
  if(Dot(n, N) > 0.0f)
  {
    T = -T;
    B = -B;
  }
*/



  float* vbuf = (float*)vb->Lock(0);
  const int nElem = 10;
  for(unsigned int i = 0; i < vertices.Size(); i++)
  {
    const Vector3f& v = vertices[i];

    // xyz
    vbuf[i*nElem+0] = v.x;
    vbuf[i*nElem+1] = v.y;
    vbuf[i*nElem+2] = v.z;

    // normal
    vbuf[i*nElem+3] = N.x;
    vbuf[i*nElem+4] = N.y;
    vbuf[i*nElem+5] = N.z;

    // tc0
    vbuf[i*nElem+6] = texcoords[i].x;
    vbuf[i*nElem+7] = texcoords[i].y;

    // tc1
    vbuf[i*nElem+8] = texcoords[i].x;
    vbuf[i*nElem+9] = texcoords[i].y;

    // center
    c += vertices[i];
  }
  vb->Unlock();

  // average vertices for center
  c /= (float)vertices.Size();





// calculate dimensions down the tangent and binormal
  width = 0;
  height = 0;
  for(unsigned int i = 0; i < vertices.Size(); i++)
  {
    Vector3f& v = vertices[i];
    float d = 0;

    d = fabs(Dot(c - v, T));
    if(d > width)
      width = d;

    d = fabs(Dot(c - v, B));
    if(d > height)
      height = d;
  }

  return 1;
}



int Surface::CreateLight(const snVector<Vector3f>& vertices, const snVector<Vector2f>& texcoords,
                         float intensity)
{
  Create("white", vertices, texcoords);
  unsigned char* p = new unsigned char[TEX_SIZE*TEX_SIZE*4];
  for(int i = 0; i < TEX_SIZE*TEX_SIZE*4; i++)
    p[i] = 255;
  resid = g_renderDevice->CreateTexture(TEX_SIZE,TEX_SIZE,PF_R8G8B8A8, 0, p);
  accum = g_renderDevice->CreateTexture(TEX_SIZE,TEX_SIZE,PF_R8G8B8A8, 0, p);
  copy->SetTexture(0, accum);
  blend->SetTexture(0,accum);
  rblend->SetTexture(0, resid);
  rcopy->SetTexture(0, resid);
  type = S_LIGHT;

  Frame(Vector3f(0,0,0));
  CopyAccumToLightMap();

  return 1;
}


void Surface::Render(int lightmap)
{
  if(!lightmap)
    g_renderer->BindMaterial(material);
  else if(lightmap == 1)
    g_renderer->BindMaterial(light);
  else if(lightmap == 2)
    g_renderer->BindMaterial(blend);
  else if(lightmap == 3)
    g_renderer->BindMaterial(copy);
  else if(lightmap == 4)
    g_renderer->BindMaterial(rblend);
  else if(lightmap == 5)
    g_renderer->BindMaterial(rcopy);

  if(vb)
    vb->Render(R_QUADS, 4, 0, 0);


/*
  g_materialSystem->BindMaterial(0);
  g_renderer->SetColor(1,0,0);
  g_renderer->DrawLine(c.x, c.y, c.z,
                       c.x+T.x*5.0, c.y+T.y*5.0, c.z+T.z*5.0);
  g_renderer->SetColor(0,1,0);
  g_renderer->DrawLine(c.x, c.y, c.z,
                       c.x+B.x*5.0, c.y+B.y*5.0, c.z+B.z*5.0);
  g_renderer->SetColor(0,0,1);
  g_renderer->DrawLine(c.x, c.y, c.z,
                       c.x+N.x*5.0, c.y+N.y*5.0, c.z+N.z*5.0);
*/
}



void Surface::Frame(Vector3f& p)
{
  // set view from light looking at
  // surface in texture space
  Transformation view;
  view.Translate() = p;
  view.Rotate().SetColumn(0, T);
  view.Rotate().SetColumn(1, B);
  view.Rotate().SetColumn(2, N);
  view = view.Inverse();
  g_renderer->SetViewMatrix(view);


  float d = Dot(-N, c - p);
  Vector3f pp = p + -N * d;

  float dT = Dot(c - pp, T);
  float dB = Dot(c - pp, B);

  if(d == 0)
    d = 1.0;
  float l = (dT - width) / d;
  float r = (dT + width) / d;
  float b = (dB - height) / d;
  float t = (dB + height) / d;
  float n = 1.0f;
  float f = 1000.0f;
  g_renderer->SetFrustum(l, r, b, t, n, f);
}


void Surface::ClearAccum()
{
  g_renderer->BindMaterial(0);
  accum->BeginRenderTarget();
  g_renderer->SetClearColor(0,0,0,0);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  accum->EndRenderTarget(((WindowedApp*)App::theApp)->GetWidth(),
                         ((WindowedApp*)App::theApp)->GetHeight());
}



void Surface::CreateLightMap(Vector3f& lp, Vector3f& ld, snVector<Surface*>& surfaces)
{
  g_materialSystem->BindMaterial(0);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  lightmap->BeginRenderTarget();

  // render all surfaces with light
  for(unsigned int j = 0; j < surfaces.Size(); j++)
  {
    if(this == surfaces[j])
      g_renderer->SetColor(1,1,1);
    else
      g_renderer->SetColor(0,0,0);

    surfaces[j]->Render(1);
  }
  // copy to lightmap
  lightmap->EndRenderTarget(((WindowedApp*)App::theApp)->GetWidth(),
                            ((WindowedApp*)App::theApp)->GetHeight());
}



void Surface::AccumulateLight()
{
  g_materialSystem->BindMaterial(0);
  g_renderer->SetColor(1,1,1);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  // blend lightmap into accum
  accum->BeginRenderTarget();
  Render(2);
  // copy to accum
  accum->EndRenderTarget(((WindowedApp*)App::theApp)->GetWidth(),
                         ((WindowedApp*)App::theApp)->GetHeight());
}



void Surface::AccumulateResidualLight()
{
  g_renderer->SetColor(1,1,1);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  // blend lightmap into accum
  resid->BeginRenderTarget();
  Render(4);
  // copy to resid
  resid->EndRenderTarget(((WindowedApp*)App::theApp)->GetWidth(),
                         ((WindowedApp*)App::theApp)->GetHeight());
}


void Surface::ClearResidualLight()
{
  g_renderer->SetColor(0,0,0);
  g_renderer->SetClearColor(0,0,0,0);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  // blend lightmap into accum
  resid->BeginRenderTarget();
//  Render(4);
  // copy to resid
  resid->EndRenderTarget(((WindowedApp*)App::theApp)->GetWidth(),
                         ((WindowedApp*)App::theApp)->GetHeight());
}


void Surface::CopyAccumToLightMap()
{
  g_renderer->SetColor(1,1,1);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  // blend lightmap into accum
  lightmap->BeginRenderTarget();
  Render(3);
  // copy to accum
  lightmap->EndRenderTarget(((WindowedApp*)App::theApp)->GetWidth(),
                            ((WindowedApp*)App::theApp)->GetHeight());
}


void Surface::CopyResidualToLightMap()
{
  g_renderer->SetColor(1,1,1);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  // blend lightmap into accum
  lightmap->BeginRenderTarget();
  Render(5);
  // copy to accum
  lightmap->EndRenderTarget(((WindowedApp*)App::theApp)->GetWidth(),
                            ((WindowedApp*)App::theApp)->GetHeight());
}



void Surface::CreateLights(snVector<Light*>& lights)
{
  float* data = resid->GetData();
  // create a light for each texel
  for(unsigned int x = 0; x < resid->GetWidth(); x++)
  {
    for(unsigned int y = 0; y < resid->GetHeight(); y++)
    {
      float* a = &data[(y*resid->GetWidth()+x)*4];
      // unpack
      float power = 0.0;
      power += a[3];
      power *= 1.0 / 256.0;
      power += a[2];
      power *= 1.0 / 256.0;
      power += a[1];
      power *= 1.0 / 256.0;
      power += a[0];

      float u = (float)(x+0.5) / (float)resid->GetWidth();
      float v = (float)(y+0.5) / (float)resid->GetHeight();

      Vector3f p(T*u*width*2.0 + B*v*height*2.0);
      p = c-T*width-B*height+p;

      if(type != S_LIGHT)
        power *= 0.75;
      lights.PushBack(new Light(p, N, power*20.0));
    }
  }

  ClearResidualLight();
}



float Surface::GetPower()
{
  // get accum buffer
  float* data = resid->GetData();
  float totalPower = 0.0f;
  int nTotal = 0;
  for(unsigned int x = 0; x < accum->GetWidth(); x++)
  {
    for(unsigned int y = 0; y < accum->GetHeight(); y++)
    {
      float* a = &data[(y*accum->GetWidth()+x)*4];

      // unpack
      float power = 0.0;
      power += a[3];
      power *= 1.0 / 256.0;
      power += a[2];
      power *= 1.0 / 256.0;
      power += a[1];
      power *= 1.0 / 256.0;
      power += a[0];

      if(power > 0)
        nTotal++;
      totalPower += power;
    }
  }

  totalPower /= (float)nTotal;

  return totalPower;
}



int Surface::GetNumPatches()
{
  return TEX_SIZE * TEX_SIZE;
}
