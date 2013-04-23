// textureMgr.cpp
#include "textureMgr.h"
#include "public/fileSystem.h"
#include "render/renderDevice.h"
#include "common/common.h"
#include "common/log.h"
#include "render/renderer_defs.h"
#include "common/global.h"
#include <stdio.h>
#include <string.h>
#include "render/materialSystem/dds.h"
#include "render/materialSystem/tga.h"
#include "render/materialSystem/texture.h"

extern IRenderDevice* g_renderDevice;



TextureMgr::TextureMgr()
{
}



TextureMgr::~TextureMgr()
{
}



int TextureMgr::Initialize()
{
  unsigned char tile[64] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  Texture* texture = g_renderDevice->CreateTexture(4, 4, PF_R8G8B8A8, 1, tile);
  textures["__blank__"] = texture;

  return true;
}


Texture* TextureMgr::GetTexture(const char* textureName)
{
  if(textures.Exists(textureName))
    return textures[textureName];

  Texture* texture = LoadTexture(textureName);
  if(texture)
    textures[textureName] = texture;

  return texture;
}



Texture* TextureMgr::LoadTexture(const char* textureName)
{
  ///////
  // @todo test for enough memory, release if necessary
  ///////
  unsigned char* buffer;
  FileHandle file = g_fileSystem->Open((char*)textureName);
  int length = g_fileSystem->GetSize(file);
  if(length <= 0)
  {
    LOG(LOG_RED, "ERROR: Texture not found '%s'", textureName);
    g_fileSystem->Close(file);
    printf("blank texture\n");
    return textures["__blank__"];
  }
  buffer = (unsigned char*)g_fileSystem->GetData(file);

  // load file
  unsigned int width;
  unsigned int height;
  unsigned int format=0;

  if(strstr(textureName, ".tga"))
  {
    buffer = ReadTGA(buffer, &width, &height, &format);
  }
  else if(strstr(textureName, ".dds"))
  {
    buffer = ReadDDS(buffer, &width, &height, &format);
  }

  Texture* texture = g_renderDevice->CreateTexture(width, height, format, 1, buffer);
  g_fileSystem->Close(file);

  return texture;
}



void TextureMgr::BindTexture(int stage, Texture* texture)
{
  if(stage < 0)
    return;

  if(!texture)
  {
//    g_renderDevice->DisableTextureStage(stage);
    g_renderDevice->BindTexture(stage, 0);
  }
  else
  {
    g_renderDevice->BindTexture(stage, texture);
  }
}



void TextureMgr::ReloadTexture(int texId)
{
  /*
  texture_t* tex = m_textureHandleMgr.Dereference(texId);
  g_common->Print("Reloading '%s'", tex->name);

  if(!tex)
    return;

  g_renderDevice->ReleaseTexture(tex->apiId);
  LoadTexture(tex->name, tex);
  printf("%s reloaded\n", tex->name);
  */
}



void TextureMgr::ListTextures()
{
  g_common->Print("\x003-- Texture List -------------------------");

  snVector<const char*> keys = textures.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
    g_common->Print("  %s", keys[i]);

  g_common->Print("\x003-- %03d Textures -------------------------", keys.Size());
}



void TextureMgr::InvalidateTexture(const char* name)
{
  /*
  if(textures.Exists(name))
    textures[name]->Invalidate();
  */
}

