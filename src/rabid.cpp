/**
  @file rabid.cpp
  @author Christopher Olsen
  @date Fri Mar 25 02:00:40 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#include "rabid.h"
#include "common/console.h"
#include "common/commandSystem.h"
#include "common/cvarSystem.h"
#include "render/meshSystem/meshSystem.h"
#include "render/materialSystem/materialSystem.h"
#include "render/materialSystem/material.h"
#include "common/common.h"
#include "render/renderer.h"
#include "common/timer.h"
#include "common/fileSystem/fileSystem.h"
#include "common/timer.h"
#include "math/vector3.h"
#include "math/transformation.h"
#include "render/renderDevice.h"
#include "render/materialSystem/texture.h"
#include "common/defMgr.h"
#include "libsn/vector.h"
#include "public/def.h"
#include "libsn/dict.h"
#include "render/surface.h"
#include "common/keys.h"
#include "math/matrix3.h"
#include <assert.h>
#include <stdio.h>

// global interfaces
Console*        g_console = 0;
ICVarSystem*    g_cvarSystem = 0;
CommandSystem*  g_commandSystem = 0;
IFileSystem*    g_fileSystem = 0;
MaterialSystem* g_materialSystem = 0;
IMeshSystem*    g_meshSystem = 0;
Common*         g_common = 0;
IRenderer*      g_renderer = 0;
Timer           g_timer;
DefMgr*         g_defMgr = 0;

extern IRenderDevice* g_renderDevice;
Material* logo = 0;

CVar r_profile("r_profile", "0");
CVar cl_camx("cl_camx", "0");
CVar cl_camy("cl_camy", "0");
CVar cl_camz("cl_camz", "50");
CVar cl_camrotx("cl_camrotx", "0");
CVar cl_camroty("cl_camroty", "0");
CVar cl_camrotz("cl_camrotz", "0");
CVar cl_surf("cl_surf", "0");
CVar r_resid("r_resid", "0");
CVar r_showbrightest("r_showbrightest", "1");

CVar npasses("npasses", "1");
CVar done("done", "1");


snVector<Surface*> surfaces;
snVector<Light*> lights;

typedef enum
{
  B_FORWARD,
  B_BACKWARD,
  B_LEFT,
  B_RIGHT,

  B_RENDER,
  B_LIGHT_MODE,
  B_TOGGLE_BRIGHTEST,

  NUM_KEYS
} eKeys;

char bindings[NUM_KEYS] = {'w', 's', 'a', 'd', ' ', K_TAB, 'b'};
char keyState[NUM_KEYS];

enum {GM_PRESS, GM_RELEASE, GM_MOUSEMOVE};
typedef struct gameMsg_s
{
  int type;
  int data;
} gameMsg_t;

int RabidEngine::initialized = RabidEngine::Initialize();

int RabidEngine::Initialize()
{
  App::Run = &WindowedApp::Run;
  theApp = new RabidEngine();
  initialized = true;
  return initialized;
}



RabidEngine::RabidEngine() :
  WindowedApp("Rabid", 0,0, 800, 600, 0)
{
  memset(keyState, 0, NUM_KEYS * sizeof(char));
}


RabidEngine::~RabidEngine()
{
}


int RabidEngine::LoadMap(const char* filename)
{
  IDef* mapDef = g_defMgr->GetDef(filename);

  if(!mapDef)
  {
    g_common->Print("\x007Map %s NOT FOUND!", filename);
    return 0;
  }

  assert(mapDef);

  const snVector<IDef*>& sections = mapDef->GetComponents();
  for(unsigned int i = 0; i < sections.Size(); i++)
  {
    // SURFACE
    if(strcmp(sections[i]->GetType(), "surface") == 0)
    {
      Surface* s = new Surface();
      surfaces.PushBack(s);
      
      const snDict& d = sections[i]->GetDict();
      if(!d.ExistsVec3Vec("vertices"))
      {
        printf("Surface contains no vertices... SKIPPING!\n");
        continue;
      }

      if(!d.ExistsVec2Vec("texcoords"))
      {
        printf("Surface contains no vertices... SKIPPING!\n");
        continue;
      }


      const snVector<Vector3f>& vertices  = d["vertices"].GetVec3Vec();
      const snVector<Vector2f>& texcoords = d["texcoords"].GetVec2Vec();

      const char* mat = "axis";
      if(d.ExistsString("material"))
        mat = d["material"].GetString();
      else
        printf("Surface material undefined!\n");
      s->Create(mat, vertices, texcoords);
    }

    // LIGHT
    else if(strcmp(sections[i]->GetType(), "light") == 0)
    {
      Surface* s = new Surface();
      const snDict& d = sections[i]->GetDict();

      float intensity = 1.0;
      if(d.ExistsFloat("intensity"))
        intensity = d["intensity"].GetFloat();

      if(!d.ExistsVec3Vec("vertices"))
      {
        printf("Surface contains no vertices... SKIPPING!\n");
        continue;
      }

      if(!d.ExistsVec2Vec("texcoords"))
      {
        printf("Surface contains no vertices... SKIPPING!\n");
        continue;
      }

      const snVector<Vector3f>& vertices  = d["vertices"].GetVec3Vec();
      const snVector<Vector2f>& texcoords = d["texcoords"].GetVec2Vec();
      s->CreateLight(vertices, texcoords, intensity);
      surfaces.PushBack(s);
    }
  }

  return 1;
}



bool RabidEngine::OnInitialize(int argc, char** argv)
{
  LOG_START("rabid_log.html");

  g_fileSystem = (IFileSystem*)new FileSystem();
  g_fileSystem->AddPath();

  g_defMgr = new DefMgr();

  g_cvarSystem = new CVarSystem();
  g_cvarSystem->Initialize();
  CVar::RegisterStaticCVars();

  g_console = new Console();
  g_console->SetScreenDimensions(GetWidth(), GetHeight());

  g_commandSystem = new CommandSystem();
  g_commandSystem->Initialize();

  g_common = new Common();

  g_renderer = CreateRenderer("GL");
  g_renderer->SetViewport(0,0, GetWidth(), GetHeight());

  g_common->Print("\x009### Rabid Hardware Radiosity Engine");
  g_common->Print("\x009### Based on the Catharsis Game Engine");
  g_common->Print("\x009### Christopher Olsen");
  g_common->Print("\x005### \x007SPACE\x005 distributes light ###");
  g_common->Print("\x005### \x007TAB\x005 switches light view modes ###");
  g_common->Print("\x005### \x007""B\x005 toggles the brightest surface ###");

  LoadMap("test.map");

  cl_camrotx.SetFloat(0.0);
  cl_camroty.SetFloat(0.0);
  cl_camrotz.SetFloat(0.0);

  logo = g_materialSystem->GetMaterial("logo");

  return true;
}



bool RabidEngine::OnTerminate()
{
  delete g_renderer;
  delete g_fileSystem;
  delete g_cvarSystem;
  delete g_commandSystem;
  delete g_console;
  delete g_common;

  return true;
}



void RabidEngine::OnIdle()
{
  g_timer.Update();
  const float dt = g_timer.GetTimeChange();

  g_console->Update(dt);




  Transformation view;
  view.Rotate().FromEulerXYZ(cl_camrotx.GetFloat(),
                             cl_camroty.GetFloat(),
                             cl_camrotz.GetFloat());

  // move
  const Vector3f forward = -view.Rotate().GetColumn(2);
  const Vector3f right   =  view.Rotate().GetColumn(0);

  const float vel = 50.0 * dt;
  if(keyState[B_FORWARD])
  {
    cl_camx.SetFloat(cl_camx.GetFloat() + forward.x * vel);
    cl_camy.SetFloat(cl_camy.GetFloat() + forward.y * vel);
    cl_camz.SetFloat(cl_camz.GetFloat() + forward.z * vel);
  }
  if(keyState[B_BACKWARD])
  {
    cl_camx.SetFloat(cl_camx.GetFloat() + -forward.x * vel);
    cl_camy.SetFloat(cl_camy.GetFloat() + -forward.y * vel);
    cl_camz.SetFloat(cl_camz.GetFloat() + -forward.z * vel);
  }
  if(keyState[B_RIGHT])
  {
    cl_camx.SetFloat(cl_camx.GetFloat() + right.x * vel);
    cl_camy.SetFloat(cl_camy.GetFloat() + right.y * vel);
    cl_camz.SetFloat(cl_camz.GetFloat() + right.z * vel);
  }
  if(keyState[B_LEFT])
  {
    cl_camx.SetFloat(cl_camx.GetFloat() + -right.x * vel);
    cl_camy.SetFloat(cl_camy.GetFloat() + -right.y * vel);
    cl_camz.SetFloat(cl_camz.GetFloat() + -right.z * vel);
  }
  if(keyState[B_RENDER])
  {
    done.Set("0");
    keyState[B_RENDER] = 0;
  }
  if(keyState[B_LIGHT_MODE])
  {
    if(r_resid.GetBool())
      r_resid.Set("0");
    else
      r_resid.Set("1");
    keyState[B_LIGHT_MODE] = 0;
  }
  if(keyState[B_TOGGLE_BRIGHTEST])
  {
    if(r_showbrightest.GetBool())
      r_showbrightest.Set("0");
    else
      r_showbrightest.Set("1");
    keyState[B_TOGGLE_BRIGHTEST] = 0;
  }






  static int pass;
  static int surf = -1;
  static int brightest;
  static int patches;
  if(done.GetBool())
  {
  }
  else
  {
    if(pass == 0)
    {
      // clear accumulation buffers
      for(unsigned int i = 0; i < surfaces.Size(); i++)
      {
//          if(surfaces[i]->GetType() != S_LIGHT)
//            surfaces[i]->ClearAccum();
      }
    }


    if(surf >= (int)surfaces.Size())
    {
      surf = -2;
      pass++;
      done.Set("1");
    }
    else if(surf == -1)
    {
      // Find Brightest Surface
      float maxPower = 0.0;
      for(unsigned int i = 0; i < surfaces.Size(); i++)
      {
        float p = surfaces[i]->GetPower();
        if(p > maxPower)
        {
          brightest = i;
          maxPower = p;
        }
      }

      for(int i = 0; i < lights.Size(); i++)
        delete lights[i];
      lights.Resize(0);

      surfaces[brightest]->CreateLights(lights);
    }
    else
    {
      Surface* lsurf = surfaces[surf];

      bool skip = false;
      // lights can't receive light
      if(lsurf->GetType() == S_LIGHT)
        skip = true;

      // surface can light itself
      if(!skip && surf == brightest)
      {
        if(r_resid.GetBool())
          lsurf->CopyResidualToLightMap();
        skip = true;
      }



      if(!skip)
      {
        // Render each sub-light's contribution
        for(unsigned int l = 0; l < lights.Size(); l++)
        {
          Vector3f& p = lights[l]->p;
          Vector3f& d = lights[l]->d;
          float     I = lights[l]->I;

          // light is on wrong side of surface
          if(Dot(p - lsurf->c, lsurf->N) < 0.1)
            continue;

          g_renderer->SetLight(0, p.x, p.y, p.z);
          g_renderer->SetLightDir(0, d.x, d.y, d.z);
          g_renderer->SetLightIntensity(0, I);
          g_renderer->SetLightFraction(0, 1.0 / (float)lights.Size());

          lsurf->Frame(p);
          lsurf->CreateLightMap(p, d, surfaces);
          lsurf->AccumulateResidualLight();
          lsurf->AccumulateLight();
          g_renderer->SetViewMatrix(0);
          patches += lsurf->GetNumPatches();
        }

        r_resid.Set(r_resid.GetBool() ? "1" : "0");
      }
    }
    surf++;
  }


  if(r_resid.Changed())
  {
    for(int i = 0; i < surfaces.Size(); i++)
    {
      Surface* lsurf = surfaces[i];
      lsurf->Frame(lsurf->c + lsurf->N*10.0);

      if(r_resid.GetBool())
        lsurf->CopyResidualToLightMap();
      else
        lsurf->CopyAccumToLightMap();
    }
  }






  // Render normal view
  view.Translate() = Vector3f(cl_camx.GetFloat(),
                              cl_camy.GetFloat(),
                              cl_camz.GetFloat());

  view = view.Inverse();
  g_renderer->SetViewport(0,0, GetWidth(),GetHeight());
  g_renderer->SetViewMatrix(view);
  g_renderer->SetProjectionMatrix(0);
  g_renderer->SetClearColor(0.25f, 0.25f, 0.35f, 1.0f);
  g_renderer->BindMaterial(0);
  g_renderer->Clear(R_COLOR_BUFFER | R_DEPTH_BUFFER);
  g_renderer->SetColor(1,1,1);


  int bsurf = 0;
  float maxPower = 0.0;
  for(unsigned int i = 0; i < surfaces.Size(); i++)
  {
    float p = surfaces[i]->GetPower();
    if(p > maxPower)
    {
      bsurf = i;
      maxPower = p;
    }
  }


  // draw all surfaces normally
  for(unsigned int i = 0; i < surfaces.Size(); i++)
  {
    if(r_showbrightest.GetBool())
    {
      if(i == bsurf)
        g_renderDevice->SetColor(1.0, 1.0, 0.7);
      else
        g_renderDevice->SetColor(1,1,1);
    }
    surfaces[i]->Render();
  }

  g_console->Draw(g_renderer);

  g_renderer->DrawTextP(15, 50, 16, r_resid.GetBool() ? "Residual" : "Accumulation");
  g_renderer->DrawTextP(15, 30, 16, "Step: %d", pass);
  g_renderer->DrawTextP(15, 10, 16, "Patches: %d", patches);
  g_materialSystem->BindMaterial(logo);
  g_renderer->DrawRect(GetWidth()-200, 0, 200, 50, 0,0,1,1);

  g_renderer->Flip();
}



void RabidEngine::Resize(int newWidth, int newHeight)
{
  WindowedApp::Resize(newWidth, newHeight);
  if(g_console)
    g_console->SetScreenDimensions(GetWidth(), GetHeight());
  
  if(g_renderer)
    g_renderer->SetViewport(0,0, GetWidth(), GetHeight());
}

bool RabidEngine::OnKeyPress(int key)
{
  if(WindowedApp::OnKeyPress(key))
    return true;

  if(key == '`')
  {
    g_console->Toggle();
    g_console->Active() ? ReleaseMouse() : CaptureMouse();
    return true;
  }

  if(g_console->Active())
  {
    return(g_console->OnKeyPress(key) != 0);
  }

  for(unsigned int i = 0; i < NUM_KEYS; i++)
  {
    if(bindings[i] == key)
      keyState[i] = 1;
  }

  return false;
}


bool RabidEngine::OnKeyRelease(int key)
{
  if(WindowedApp::OnKeyRelease(key))
    return true;

  if(g_console->Active())
  {
    return (g_console->OnKeyRelease(key) != 0);
  }

  for(unsigned int i = 0; i < NUM_KEYS; i++)
  {
    if(bindings[i] == key)
      keyState[i] = 0;
  }

  return false;
}


bool RabidEngine::OnMouseMove(int dx, int dy)
{
  if(!g_console->Active())
  {
    {
      cl_camroty.SetFloat(cl_camroty.GetFloat() - dx);
      cl_camrotx.SetFloat(cl_camrotx.GetFloat() - dy);

      while(cl_camroty.GetFloat() > 360.0f)
        cl_camroty.SetFloat(cl_camroty.GetFloat() - 360.0f);
      while(cl_camroty.GetFloat() < 0.0f)
        cl_camroty.SetFloat(cl_camroty.GetFloat() + 360.0f);

      if(cl_camrotx.GetFloat() > 90)
        cl_camrotx.SetFloat(90.0);
      if(cl_camrotx.GetFloat() < -90)
        cl_camrotx.SetFloat(-90.0);
    }

    
    return true;
  }

  return false;
}

