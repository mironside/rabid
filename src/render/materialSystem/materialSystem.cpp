// MaterialSystem.cpp
#include "materialSystem.h"

#include "public/fileSystem.h"
#include "render/renderDevice.h"
#include "common/log.h"
#include "render/renderer_defs.h"
#include "common/common.h"
#include "common/commandSystem.h"
#include "libsn/vector.h"
#include <stdio.h>
#include <string.h>
#include "material.h"
#include "common/defMgr.h"
#include "public/def.h"
#include "libsn/dict.h"
#include <assert.h>
#include "common/global.h"
#include "render/materialSystem/textureMgr.h"
extern DefMgr*        g_defMgr;
extern IRenderDevice* g_renderDevice;
TextureMgr* g_textureMgr = 0;

void cmdListMaterials(const char* args)
{
  g_materialSystem->ListMaterials();
}

void cmdListTextures(const char* args)
{
  g_materialSystem->ListTextures();
}



MaterialSystem::MaterialSystem()
{
  g_commandSystem->AddCommand("material_list", cmdListMaterials);
  g_commandSystem->AddCommand("texture_list", cmdListTextures);
}



MaterialSystem::~MaterialSystem()
{
  snVector<const char*> keys = materials.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
    delete materials[keys[i]];
  delete g_textureMgr;
}



int MaterialSystem::Initialize()
{
  g_textureMgr = new TextureMgr();
  g_textureMgr->Initialize();


  // create blank material
  Material* m = new Material();
  materials["__blank__"] = m;
  m->SetShader(0);
  m->SetTexture(0, 0);
  m->SetTextureState(0, 0);
  m->SetTexture(1, 0);
  m->SetTextureState(1, 0);
  m->SetTexture(2, 0);
  m->SetTextureState(2, 0);
  m->SetTexture(3, 0);
  m->SetTextureState(3, 0);
  RenderState* rs = g_renderDevice->GetRenderState(R_ONE, R_ZERO, R_ADD,
                                                   R_ALWAYS, 0.0, R_LT, R_ENABLE,
                                                   R_CULL_NONE);
  m->SetRenderState(rs);

  TextureState* ts = g_renderDevice->GetTextureState(R_COMBINE_MODULATE, R_COMBINE_MODULATE,
                                                     R_WRAP_REPEAT, R_FILTER_LINEAR);
  m->SetTextureState(0, ts);

  return 1;
}



Material* MaterialSystem::GetMaterial(const char* materialName, int unique)
{

  char name[256];
  // in root directory (defined in default.material)
  if(!strstr(materialName, "/") && !strstr(materialName, "."))
  {
    DefMgr::GetName("default.material", materialName, "material", name);
  }
  // in a directory, no extension
  else if(!strstr(materialName, "."))
  {
    int start = 0;
    int end = strlen(name);
    strcpy(name, materialName);
    for(int i = strlen(name); i >= 0; i--)
    {
      if(name[i] == '/')
      {
        end = i;
        for(int j = i; j >= 0; j--)
        {
          if(name[j] == '/')
            break;
          start = j;
        }
      }
    }
    memset(name, 0, 256);
    strncpy(name, materialName, end+1);
    strncat(name, &materialName[start], end-start);
    sprintf(name, "%s.material:%s", name, &materialName[end+1]);
  }
  // directory and extension, leave as is
  else
  {
    strcpy(name, materialName);
  }


  // Return material if it already exists
  if(!unique && materials.Exists(name))
    return materials[name];


  // Create material
  Material* m = new Material();
  if(!LoadMaterial(m, name))
  {
    /// @todo return an error material?!
    delete m;
    return 0;
  }

  if(!unique)
    materials[name] = m;

  return m;
}





int MaterialSystem::LoadMaterial(Material* material,
                                 const char* materialName)
{
  IDef* def = g_defMgr->GetDef(materialName);

  if(def)
    return material->CreateFromDef(material, def);
  else
    return material->CreateFromTexture(material, materialName);
}


void MaterialSystem::BindMaterial(Material* m)
{
  FN(0, "MaterialSystem::BindMaterial");
  if(!m)
  {
    g_renderDevice->DisableAllTextureStages();
    RenderState* rs = g_renderDevice->GetRenderState(R_ONE, R_ZERO, R_ADD,
                                                     R_ALWAYS, 0.0, R_LT,
                                                     R_ENABLE, R_CULL_BACK);
    g_renderDevice->BindRenderState(rs);
    return;
  }

  g_renderDevice->BindShaderProgram(m->GetShader());
  g_renderDevice->BindRenderState(m->GetRenderState());

  for(int i = 0; i < 16; i++)
  {
    if(!m->GetTextureState(i))
    {
      g_renderDevice->DisableTextureStage(i);
      continue;
    }

    /// HUGE NOTE!
    /// It seems that the texture state MUST be set AFTER
    /// the texture has been bound!  DON'T FUCK IT UP!
    g_renderDevice->EnableTextureStage(i);
    g_renderDevice->BindTexture(i, m->GetTexture(i));
    g_renderDevice->BindTextureState(i, m->GetTextureState(i));
  }
}



void MaterialSystem::ReloadMaterial(int materialId)
{
  Material* m = (Material*)materialId;
  if(!m)
    return;

  assert(g_textureMgr);
  for(int i = 0; i < 4; i++)
    g_textureMgr->ReloadTexture((int)m->GetTexture(i));

  //m_shaderProgramMgr.ReloadShaderProgram(m->GetShader());
  LoadMaterial(m, "__blank__");
}



void MaterialSystem::ListMaterials()
{
  g_common->Print("\x003-- Material List -------------------------");

  snVector<const char*> keys = materials.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
    g_common->Print("  %s", keys[i]);

  g_common->Print("\x003-- %03d Materials -------------------------", keys.Size());
}



void MaterialSystem::ListTextures()
{
  assert(g_textureMgr);
  g_textureMgr->ListTextures();
}



void MaterialSystem::InvalidateTexture(const char* name)
{
  assert(g_textureMgr);
  g_textureMgr->InvalidateTexture(name);
}







