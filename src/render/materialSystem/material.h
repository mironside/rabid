// material.h
#ifndef __MATERIAL__
#define __MATERIAL__

class IDef;
class Texture;
class RenderState;
class ShaderProgram;
class TextureState;
class TextureMod;

#include "libsn/vector.h"
#include "math/vector4.h"
#include "math/vector3.h"



#define MAX_STAGES 16

typedef struct matStage_s
{
  Texture* texture;
  TextureState* textureState;
} matStage_t;

typedef enum
{
  W_NONE,
  W_SIN,
  W_SAW,
  W_INVSAW,
  W_TRIANGLE
} eWaveType;

typedef enum
{
  TM_SCALE,
  TM_SCROLL,
  TM_ROTATE,
  TM_COLOR,
  TM_ALPHA
} eTexModType;


typedef struct wave_s
{
  eWaveType type;
  Vector4f parms;
} wave_t;

typedef struct texMod_s
{
  eTexModType type;
  Vector3f parms;
  wave_t wave;
} texMod_t;


class Material
{
protected:
  snVector<texMod_t*> mods;
  matStage_t stages[MAX_STAGES];
  RenderState* renderState;
  ShaderProgram* shader;
  unsigned int nStages;


  /// Loads a texture from a material def
  Texture* GetTexture(const IDef* def);

  /// Loads Texture State from a material def
  TextureState* GetTextureState(const IDef* def);

  /// Loads Render State from a material def
  RenderState* GetRenderState(const IDef* def);

  /// Loads Shader Program from a material def
  ShaderProgram* GetShaderProgram(const IDef* def);

  /// Creates a ShaderProgram from a catharsis shader file
  int LoadShader(ShaderProgram* s, const char* filename);


public:
  Material();
  virtual ~Material();

  RenderState* GetRenderState() { return renderState; }
  void SetRenderState(RenderState* rs) { renderState = rs; }

  ShaderProgram* GetShader() { return shader; }
  void SetShader(ShaderProgram* s) { shader = s; }

  TextureState* GetTextureState(unsigned int s)
  { return s < MAX_STAGES ? stages[s].textureState : 0; }
  void SetTextureState(unsigned int i, TextureState* ts)
  { if(i < MAX_STAGES) stages[i].textureState = ts; }

  Texture* GetTexture(unsigned int s)
  { return s < MAX_STAGES ? stages[s].texture : 0; }
  void SetTexture(unsigned int i, Texture* t)
  { if(i < MAX_STAGES) stages[i].texture = t; }

  void AddTextureMod(unsigned int s, texMod_t* tm)
  { mods.PushBack(tm); }


  /// Fills out the given material with data provided in a def
  int CreateFromDef(Material* material, const IDef* def);

  /// Fills out the given material from a texture
  int CreateFromTexture(Material* material, const char* materialName);
};

#endif
