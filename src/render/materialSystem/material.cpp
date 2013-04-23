// material.cpp
#include "material.h"
#include "public/fileSystem.h"
#include "render/renderer_defs.h"
#include "common/log.h"
#include <string.h>
#include "tga.h"
#include "dds.h"
#include "renderState.h"
#include "textureState.h"
#include "shaderProgram.h"
#include "public/def.h"
#include "libsn/dict.h"
#include <stdio.h>
#include <assert.h>
#include "render/renderDevice.h"
#include "textureMgr.h"
extern IRenderDevice* g_renderDevice;
extern TextureMgr* g_textureMgr;

Material::Material() :
  renderState(0), shader(0), nStages(0)
{
  for(unsigned int i = 0; i < MAX_STAGES; i++)
  {
    stages[i].texture = 0;
    stages[i].textureState = 0;
  }
}



Material::~Material()
{
}



unsigned int GetBlend(const char* s)
{
  if(strcmp(s, "one") == 0)
    return R_ONE;
  else if(strcmp(s, "zero") == 0)
    return R_ZERO;
  else if(strcmp(s, "src_alpha") == 0)
    return R_SRC_ALPHA;
  else if(strcmp(s, "one_minus_src_alpha") == 0)
    return R_ONE_MINUS_SRC_ALPHA;
  else if(strcmp(s, "dst_alpha") == 0)
    return R_DST_ALPHA;
  else if(strcmp(s, "one_minus_dst_alpha") == 0)
    return R_ONE_MINUS_DST_ALPHA;
  else if(strcmp(s, "src_color") == 0)
    return R_SRC_COLOR;
  else if(strcmp(s, "dst_color") == 0)
    return R_DST_COLOR;
  else if(strcmp(s, "one_minus_src_color") == 0)
    return R_ONE_MINUS_SRC_COLOR;
  else if(strcmp(s, "one_minus_dst_color") == 0)
    return R_ONE_MINUS_DST_COLOR;

  return R_ONE;
}


unsigned int GetEnable(const char* s)
{
  if(strcmp(s, "disable") == 0)
    return R_DISABLE;
  else if(strcmp(s, "enable") == 0)
    return R_ENABLE;

  return R_ENABLE;
}


unsigned int GetCull(const char* s)
{
  if(strcmp(s, "front") == 0)
    return R_CULL_FRONT;
  else if(strcmp(s, "back") == 0)
    return R_CULL_BACK;
  else if(strcmp(s, "none") == 0)
    return R_CULL_NONE;

  return R_CULL_BACK;
}


unsigned int GetBlendFunc(const char* s)
{
  if(strcmp(s, "add") == 0)
    return R_ADD;
  else if(strcmp(s, "subtract") == 0)
    return R_SUBTRACT;

  return R_ADD;
}


unsigned int GetTest(const char* s)
{
  /*
  #define R_GT      0x01
  #define R_LT      0x02
  #define R_GTE     0x03
  #define R_LTE     0x04
  #define R_EQ      0x05
  #define R_NEQ     0x06
  #define R_ALWAYS  0x07
  #define R_NEVER   0x08
  */

  if(strcmp(s, "always") == 0)
    return R_ALWAYS;
  else if(strcmp(s, "gt") == 0)
    return R_GT;
  else if(strcmp(s, "lt") == 0)
    return R_LT;
  else if(strcmp(s, "gte") == 0)
    return R_GTE;
  else if(strcmp(s, "lte") == 0)
    return R_LTE;
  else if(strcmp(s, "eq") == 0)
    return R_EQ;
  else if(strcmp(s, "neq") == 0)
    return R_NEQ;
  else if(strcmp(s, "never") == 0)
    return R_NEVER;

  return R_ALWAYS;
}

unsigned int GetCombiner(const char* s)
{
  /*
  #define R_COMBINE_REPLACE  0x01
  #define R_COMBINE_MODULATE 0x02
  #define R_COMBINE_ADD      0x03
  #define R_COMBINE_SUBTRACT 0x04
  #define R_COMBINE_DOT3     0x05
  */
  if(strcmp(s, "replace") == 0)
    return R_COMBINE_REPLACE;
  else if(strcmp(s, "modulate") == 0)
    return R_COMBINE_MODULATE;
  else if(strcmp(s, "interpolate") == 0)
    return R_COMBINE_INTERPOLATE;
  else if(strcmp(s, "add") == 0)
    return R_COMBINE_ADD;
  else if(strcmp(s, "subtract") == 0)
    return R_COMBINE_SUBTRACT;
  else if(strcmp(s, "dot3") == 0)
    return R_COMBINE_DOT3;

  return R_COMBINE_MODULATE;
}


unsigned int GetFilter(const char* s)
{
  if(strcmp(s, "linear") == 0)
    return R_FILTER_LINEAR;
  else if(strcmp(s, "nearest") == 0)
    return R_FILTER_NEAREST;

  return R_FILTER_LINEAR;
}


unsigned int GetWrap(const char* s)
{
  if(strcmp(s, "repeat") == 0)
    return R_WRAP_REPEAT;
  else if(strcmp(s, "clamp") == 0)
    return R_WRAP_CLAMP;

  return R_WRAP_REPEAT;
}


RenderState* Material::GetRenderState(const IDef* def)
{
  assert(def);

  const snDict& d = def->GetDict();
  int srcBlend = R_ONE;
  int dstBlend = R_ZERO;
  int blendFunc = R_ADD;
  int alphaTest = R_ALWAYS;
  float alphaRef = 1.0f;
  int depthTest = R_LTE;
  int depthWrite = R_ENABLE;
  int cullMode = R_CULL_BACK;
  ShaderProgram* shaderProgram = 0;

  if(d.ExistsString("srcblend"))
    srcBlend = GetBlend(d["srcblend"].GetString());
  if(d.ExistsString("dstblend"))
    dstBlend = GetBlend(d["dstblend"].GetString());
  if(d.ExistsString("blendfunc"))
    blendFunc = GetBlendFunc(d["blendfunc"].GetString());
  if(d.ExistsString("alphatest"))
    alphaTest = GetTest(d["alphatest"].GetString());
  if(d.ExistsFloat("alpharef"))
    alphaRef = d["alpharef"].GetFloat();
  if(d.ExistsString("depthtest"))
    depthTest = GetTest(d["depthtest"].GetString());
  if(d.ExistsString("depthwrite"))
    depthWrite = GetEnable(d["depthwrite"].GetString());
  if(d.ExistsString("cull"))
    cullMode = GetCull(d["cull"].GetString());

  RenderState* renderState = g_renderDevice->GetRenderState(srcBlend, dstBlend, blendFunc, alphaTest,
                                                            alphaRef, depthTest, depthWrite, cullMode);
  return renderState;
}


ShaderProgram* Material::GetShaderProgram(const IDef* def)
{
  assert(def);

  const snDict& d = def->GetDict();
  ShaderProgram* shaderProgram = 0;
  if(d.ExistsString("shader"))
  {
    shaderProgram = g_renderDevice->CreateShaderProgram();
    LoadShader(shaderProgram, d["shader"].GetString());
  }

  return shaderProgram;
}



Texture* Material::GetTexture(const IDef* def)
{
  assert(def);

  const snDict& d = def->GetDict();

  char name[256];
  def->GetRelativePath(name, def->GetName());
  Texture* t = g_textureMgr->GetTexture(name);

  return t;
}



TextureState* Material::GetTextureState(const IDef* def)
{
  assert(def);
  const snDict& d = def->GetDict();

  unsigned int colorCombiner = R_COMBINE_MODULATE;
  unsigned int alphaCombiner = R_COMBINE_MODULATE;
  unsigned int filterMode    = R_FILTER_LINEAR;
  unsigned int wrapMode      = R_WRAP_REPEAT;

  if(d.ExistsString("colorcombiner"))
    colorCombiner = GetCombiner(d["colorcombiner"].GetString());
  if(d.ExistsString("alphacombiner"))
    alphaCombiner = GetCombiner(d["alphacombiner"].GetString());
  if(d.ExistsString("filter"))
    filterMode = GetFilter(d["filter"].GetString());
  if(d.ExistsString("wrap"))
    wrapMode = GetWrap(d["wrap"].GetString());

  TextureState* ts = g_renderDevice->GetTextureState(colorCombiner, alphaCombiner,
                                                     wrapMode, filterMode);
  return ts;
}



texMod_t* GetTextureMod(const IDef* def)
{
  assert(def);

  if(strcmp(def->GetName(), "mod") != 0)
    return 0;


  const snDict& d = def->GetDict();
  texMod_t* tm = new texMod_t;
  memset(tm, 0, sizeof(texMod_t));
  const char* name = def->GetName();

  if(strcmp(name, "scroll") == 0)
  {
    tm->type = TM_SCROLL;
    if(d.ExistsVec2("dir"))
    {
      Vector2f dir = d["dir"].GetVec2();
      tm->parms = Vector3f(dir.x, dir.y, 0.0f);
    }
  }

  else if(strcmp(name, "scale") == 0)
  {
    tm->type = TM_SCALE;
    if(d.ExistsVec2("scale"))
    {
      Vector2f s = d["scale"].GetVec2();
      tm->parms = Vector3f(s.x, s.y, 0.0f);
    }
  }

  else if(strcmp(name, "rotate") == 0)
  {
    tm->type = TM_ROTATE;
    if(d.ExistsFloat("rotate"))
    {
      float r = d["rotate"].GetFloat();
      tm->parms = Vector3f(r, 0.0f, 0.0f);
    }
  }

  else
  {
    delete tm;
    return 0;
  }

  // Get the wave if it exists
  if(d.ExistsVec4("wave_sin"))
  {
    tm->wave.type = W_SIN;
    tm->wave.parms = d["wave_sin"].GetVec4();
  }
  else if(d.ExistsVec4("wave_saw"))
  {
    tm->wave.type = W_SAW;
    tm->wave.parms = d["wave_saw"].GetVec4();
  }
  else if(d.ExistsVec4("wave_invsaw"))
  {
    tm->wave.type = W_INVSAW;
    tm->wave.parms = d["wave_invsaw"].GetVec4();
  }
  else if(d.ExistsVec4("wave_triangle"))
  {
    tm->wave.type = W_TRIANGLE;
    tm->wave.parms = d["wave_triangle"].GetVec4();
  }

  return tm;
}



int Material::CreateFromDef(Material* material, const IDef* def)
{
  assert(def);

  // Render State
  RenderState* rs = GetRenderState(def);
  material->SetRenderState(rs);

  // Shader Program
  ShaderProgram* sp = GetShaderProgram(def);
  material->SetShader(sp);

  // Texture Stages
  int stage = 0;
  const snVector<IDef*> defs = def->GetComponents();
  for(unsigned int i = 0; i < defs.Size(); i++)
  {
    if(strcmp(defs[i]->GetType(), "texmap") == 0)
    {
      // Texture
      Texture* t = GetTexture(defs[i]);
      material->SetTexture(stage, t);

      // Texture State
      TextureState* ts = GetTextureState(defs[i]);
      material->SetTextureState(stage, ts);


      // Subsections
      const snVector<IDef*>& sections = defs[i]->GetComponents();
      for(unsigned int j = 0; j < sections.Size(); j++)
      {
        // Texture Mod
        // there can be more than one
        if(strcmp(sections[j]->GetType(), "mod") == 0)
        {
          texMod_t* tm = GetTextureMod(sections[j]);
          if(tm)
            material->AddTextureMod(i, tm);
        }
      }

      stage++;
    }
  }

  return 1;
}




int Material::CreateFromTexture(Material* material, const char* materialName)
{
  /// Create Material From Texture
  // otherwise make a single stage material with texture of the same name.
  char texName[256];
  char relName[256];
  assert(materialName);

  /// Transform the material def name into a texture name
  /// 'ui/themes/grey/grey.def:button => 'ui/themes/grey/button
  int end = 0;
  for(int i = strlen(materialName); i >= 0; i--)
  {
    if(materialName[i] == '/')
    {
      end = i+1;
      break;
    }
  }
  memset(relName, 0, 256);
  strncpy(relName, materialName, end);
  materialName = strstr(materialName, ":")+1;
  strcat(relName, materialName);
  sprintf(texName, "%s.tga", relName);

  /// @todo Texture Manager should only take the name of the
  /// texture (excluding extension), the Texture Manager would
  /// then load whatever format existed


  // use blank texture if texture file doesn't exist
  if(!g_fileSystem->Exists(texName))
    material->SetTexture(0, g_textureMgr->GetTexture("__blank__"));
  else
    material->SetTexture(0, g_textureMgr->GetTexture(texName));


  // Texture State
  TextureState* ts = g_renderDevice->GetTextureState(R_COMBINE_MODULATE, R_COMBINE_MODULATE,
                                                      R_WRAP_REPEAT, R_FILTER_LINEAR);
  material->SetTextureState(0, ts);


  // Add normal map <texname>_normal, if file exists
  sprintf(texName, "%s_normal.dds", relName);
  if(g_fileSystem->Exists(texName))
  {
    // Texture
    material->SetTexture(1, g_textureMgr->GetTexture(texName));

    // Texture State
    TextureState* ts = g_renderDevice->GetTextureState(R_COMBINE_MODULATE, R_COMBINE_MODULATE,
                                                        R_WRAP_REPEAT, R_FILTER_LINEAR);
    material->SetTextureState(1, ts);

    // Shader Program
    ShaderProgram* s = g_renderDevice->CreateShaderProgram();
    LoadShader(s, "shader/normalmap.shader");
    material->SetShader(s);
  }

  // Render State
  RenderState* rs = g_renderDevice->GetRenderState(R_ONE, R_ZERO, R_ADD,
                                                    R_ALWAYS, 0.0, R_LTE,
                                                    R_ENABLE, R_CULL_BACK);
  material->SetRenderState(rs);

  return 1;
}



int Material::LoadShader(ShaderProgram* p, const char* filename)
{
  FileHandle file = g_fileSystem->Open((char*)filename);
  if(file < 0)
    return 0;
  int length = g_fileSystem->GetSize(file);
  char* data = (char*)g_fileSystem->GetData(file);


  char* glsl = strstr(data, "<GLSL>");
  char* hlsl = strstr(data, "<HLSL>");
  char* end  = &data[length];
  if(!glsl)
  {
    printf("*** RENDERER ERROR: \'%s\' does not have a GLSL implementation\n", filename);
    return 0;
  }

  // using OpenGL so 'end' the file at the start of the <HLSL> section (if it's below)
  if(hlsl > glsl)
  {
    end = hlsl;
  }

  char* vs = strstr(data, "[VertexShader]");
  char* fs = strstr(data, "[FragmentShader]");

  if(vs)
  {
    vs += strlen("[VertexShader]");
    int length = 0;
    if(fs)
      length = fs - vs;
    else
      length = end - vs;

    p->LoadVertexShader(vs, length);
  }

  if(fs)
  {
    fs += strlen("[FragmentShader]");

    int length = end - fs;
    p->LoadFragmentShader(fs, length);
  }

  g_fileSystem->Close(file);

  return 1;
}
