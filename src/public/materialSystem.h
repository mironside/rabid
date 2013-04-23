// MaterialSystem.h
#ifndef __IMATERIAL_SYSTEM__
#define __IMATERIAL_SYSTEM__

class Material;
class IMaterialSystem
{
public:
  virtual ~IMaterialSystem() {}
  virtual int Initialize() = 0;
  virtual Material* GetMaterial(const char* materialName, int unique=0) = 0;
  virtual void BindMaterial(Material* m) = 0;
  virtual void ReloadMaterial(int materialId) = 0;
  virtual void InvalidateTexture(const char* name) = 0;
};

#ifdef GAME
extern IMaterialSystem* g_materialSystem;
#endif
#endif


