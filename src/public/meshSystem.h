/**
  @file meshSystem.h
  @author Christopher Olsen
  @date Mon Dec  6 02:36:24 2004


*/
#ifndef __IMESH_SYSTEM__
#define __IMESH_SYSTEM__

class IMesh;
class DynamicMeshInstance;

class IMeshSystem
{
public:
  virtual ~IMeshSystem() {};
  virtual void Initialize() = 0;
  virtual void Release() = 0;

  virtual IMesh* GetMesh(const char* meshName) = 0;
  virtual void FreeMesh(IMesh* mesh) = 0;
  virtual IMesh* DefaultMesh() = 0;
  virtual void Print() = 0;
};

extern IMeshSystem* g_meshSystem;


#endif

