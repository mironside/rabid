// MaterialSystem.h
#ifndef __MATERIAL_SYSTEM__
#define __MATERIAL_SYSTEM__

#include "public/materialSystem.h"
#include "libsn/hashMap.h"


class Material;
class ShaderProgram;
class IDef;

/// @todo get rid of conversions from int materialIds to
/// material pointer (They may still be around).
class MaterialSystem : public IMaterialSystem
{
protected:
  snSHashMap<Material*> materials;

  /// Loads a material from a def if it exists or a texture,
  /// also used when reloading a material
  int LoadMaterial(Material* material, const char* materialName);




public:
  MaterialSystem();
  virtual ~MaterialSystem();

  int Initialize();

  Material* GetMaterial(const char* materialName, int unique=0);
  void BindMaterial(Material* m);

  void ReloadMaterial(int materialId);
  void ListMaterials();
  void ListTextures();
  void InvalidateTexture(const char* name);
};

extern MaterialSystem* g_materialSystem;

#endif


